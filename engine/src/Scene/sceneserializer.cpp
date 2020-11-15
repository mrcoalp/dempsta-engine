#include "Scene/sceneserializer.h"

#include "Core/assetsmanager.h"
#include "Scene/components.h"
#include "Scene/entity.h"
#include "Utils/fileutils.h"

namespace de {
template <typename Component>
static void AddComponentToJSON(Entity entity, const std::function<void(Component&)>& handleComponent) {
    if (entity.HasComponent<Component>()) {
        handleComponent(entity.GetComponent<Component>());
    }
}

void SceneSerializer::Serialize(const std::string& filePath) const {
    JSON::Scene jScene;
    jScene.name = "todo_scene_name";
    const auto& assets = AssetsManager::GetInstance().GetAssets();
    for (const auto& asset : AssetsManager::GetInstance().GetTracker()) {
        JSON::Asset jAsset{{false}, (int)assets[asset.second]->GetType(), assets[asset.second]->GetFilePath(), asset.first};
        if (assets[asset.second]->GetType() == AssetType::Font) {
            jAsset.fontSize = AssetsManager::GetInstance().GetFont(asset.first)->GetSize();
        }
        if (assets[asset.second]->GetType() == AssetType::Atlas) {
            jAsset.cellSize = JSON::Vec2(AssetsManager::GetInstance().GetAtlas(asset.first)->GetCellSize());
        }
        jScene.assets.emplace_back(jAsset);
    }
    m_scene->ForEachEntity([&](const auto& entityId) {
        auto entity = Entity(entityId, m_scene.get());
        JSON::Entity jEntity;
        AddComponentToJSON<IDComponent>(entity, [&jEntity](IDComponent& component) { jEntity.idComponent = {{false}, (uint64_t)component.uuid}; });
        AddComponentToJSON<NameComponent>(entity, [&jEntity](NameComponent& component) { jEntity.nameComponent = {{false}, component.name}; });
        AddComponentToJSON<TransformComponent>(entity, [&jEntity](TransformComponent& component) {
            JSON::Vec3 translation{component.translation};
            JSON::Vec3 rotation{component.rotation};
            JSON::Vec3 scale{component.scale};
            jEntity.transformComponent = {{false}, translation, rotation, scale};
        });
        AddComponentToJSON<CameraComponent>(entity, [&jEntity](CameraComponent& component) {
            const auto& camera = component.camera;
            JSON::SceneCamera jSceneCamera;
            jSceneCamera.ortho = {{false}, camera.GetOrthographicSize(), camera.GetOrthographicNearClip(), camera.GetOrthographicFarClip()};
            jEntity.cameraComponent = {{false}, component.primary, component.fixedAspectRatio, jSceneCamera};
        });
        AddComponentToJSON<SpriteComponent>(entity, [&jEntity](SpriteComponent& component) {
            bool hasSprite = false;
            std::vector<JSON::Vec2> spriteCoords;
            if (component.sprite != nullptr) {
                hasSprite = true;
                for (size_t i = 0; i < 4; ++i) {
                    spriteCoords.emplace_back(JSON::Vec2(component.sprite->GetCoordinates()[i]));
                }
            }
            jEntity.spriteComponent = {{false}, JSON::Vec4(component.color), JSON::Vec2(component.anchor), component.asset, hasSprite, spriteCoords};
        });
        AddComponentToJSON<ScriptComponent>(entity, [&jEntity](ScriptComponent& component) { jEntity.scriptComponent = {{false}, component.asset}; });
        AddComponentToJSON<SoundComponent>(entity, [&jEntity](SoundComponent& component) { jEntity.soundComponent = {{false}, component.asset}; });
        AddComponentToJSON<LabelComponent>(entity, [&jEntity](LabelComponent& component) {
            jEntity.labelComponent = {{false}, component.asset};
            if (component.label != nullptr) {
                jEntity.labelComponent.content = component.label->GetContent();
            }
        });
        jScene.entities.push_back(jEntity);
    });
    JSON::WriteFile(jScene, filePath);
}

bool SceneSerializer::Deserialize(const std::string& filePath) const {
    JSON::Scene jScene;
    if (!JSON::ReadFile(filePath, jScene)) {
        return false;
    }
    LOG_ENGINE_TRACE("Deserializing scene '{}'...", jScene.name);
    auto& assetsManager = AssetsManager::GetInstance();
    for (const auto& asset : jScene.assets) {
        if (!assetsManager.Exists(asset.name)) {
            switch ((AssetType)asset.type) {
                case AssetType::Atlas:
                    assetsManager.AddAtlas(asset.name, asset.path, asset.cellSize.ToGLM());
                    break;
                case AssetType::Font:
                    assetsManager.AddFont(asset.name, asset.path, asset.fontSize);
                    break;
                case AssetType::Sound:
                    assetsManager.AddSound(asset.name, asset.path);
                    break;
                case AssetType::Script:
                    assetsManager.AddScript(asset.name, asset.path);
                    break;
                case AssetType::Shader:
                    // TODO(mpinto)
                    break;
                case AssetType::Undefined:
                default:
                    break;
            }
        }
    }
    for (const auto& entity : jScene.entities) {
        uint64_t uuid = !entity.idComponent.is_empty ? entity.idComponent.id : 0;
        std::string name = !entity.nameComponent.is_empty ? entity.nameComponent.name : "New Entity";
        auto deserialized = m_scene->CreateEntityWithID(UUID(uuid), name, false);
        if (!entity.transformComponent.is_empty) {
            const auto& saved = entity.transformComponent;
            auto& tc = deserialized.AddComponent<TransformComponent>();
            tc.translation = saved.translation.ToGLM();
            tc.rotation = saved.rotation.ToGLM();
            tc.scale = saved.scale.ToGLM();
        }
        if (!entity.cameraComponent.is_empty) {
            const auto& saved = entity.cameraComponent;
            auto& cc = deserialized.AddComponent<CameraComponent>();
            cc.primary = saved.primary;
            cc.fixedAspectRatio = saved.fixedAspectRatio;
            if (!saved.sceneCamera.ortho.is_empty) {
                const auto& ortho = saved.sceneCamera.ortho;
                cc.camera.SetOrthographic(ortho.size, ortho.nearClip, ortho.farClip);
            }
        }
        if (!entity.spriteComponent.is_empty) {
            const auto& saved = entity.spriteComponent;
            auto& sc = deserialized.AddComponent<SpriteComponent>(saved.asset);
            sc.color = saved.color.ToGLM();
            sc.anchor = saved.anchor.ToGLM();
            if (saved.hasSprite) {
                sc.sprite = assetsManager.CreateSprite(saved.asset);
                glm::vec2 coords[4];
                for (size_t i = 0; i < saved.spriteCoords.size(); ++i) {
                    coords[i] = saved.spriteCoords[i].ToGLM();
                }
                sc.sprite->SetCoordinates(coords);
            }
        }
        if (!entity.scriptComponent.is_empty) {
            deserialized.AddComponent<ScriptComponent>(entity.scriptComponent.asset);
        }
        if (!entity.soundComponent.is_empty) {
            const auto& asset = entity.soundComponent.asset;
            deserialized.AddComponent<SoundComponent>(asset).sound = assetsManager.CreateSoundInstance(asset);
        }
        if (!entity.labelComponent.is_empty) {
            const auto& asset = entity.labelComponent.asset;
            const auto& content = entity.labelComponent.content;
            deserialized.AddComponent<LabelComponent>(asset).label = CreateRef<Label>(assetsManager.GetFont(asset), content);
        }
    }
    LOG_ENGINE_TRACE("Deserialized {} entities", jScene.entities.size());
    return true;
}
}  // namespace de
