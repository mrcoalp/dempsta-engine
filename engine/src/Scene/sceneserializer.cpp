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
    jScene.id = "todo_scene_name";
    const auto& assets = AssetsManager::GetInstance().GetAssets();
    for (const auto& asset : AssetsManager::GetInstance().GetTracker()) {
        JSON::Asset jAsset{{false}, (int)assets[asset.second]->GetType(), assets[asset.second]->GetFilePath(), asset.first};
        if (assets[asset.second]->GetType() == AssetType::Font) {
            jAsset.fontSize = AssetsManager::GetInstance().GetFont(asset.first)->GetSize();
        }
        jScene.assets.emplace_back(jAsset);
    }
    m_scene->ForEachEntity([&](const auto& entityId) {
        auto entity = Entity(entityId, m_scene.get());
        JSON::Entity jEntity;
        jEntity.id = 123;
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
            jEntity.spriteComponent = {{false}, JSON::Vec4(component.color), JSON::Vec2(component.anchor), component.asset};
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
    LOG_ENGINE_TRACE("Deserializing scene '{}'...", jScene.id);
    for (const auto& asset : jScene.assets) {
        auto& assetsManager = AssetsManager::GetInstance();
        switch ((AssetType)asset.type) {
            case AssetType::Sprite:
                assetsManager.AddSprite(asset.name, asset.path);
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
                // assetsManager.AddSprite(asset.name, asset.path);
                break;
            case AssetType::Undefined:
            default:
                break;
        }
    }
    for (const auto& entity : jScene.entities) {
        auto uuid = entity.id;  // todo(mpinto): UUID
        std::string name;
        if (!entity.nameComponent.is_empty) {
            name = entity.nameComponent.name;
        }
        auto deserialized = m_scene->CreateEntity(name, false);
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
        }
        if (!entity.scriptComponent.is_empty) {
            deserialized.AddComponent<ScriptComponent>(entity.scriptComponent.asset);
        }
        if (!entity.soundComponent.is_empty) {
            const auto& asset = entity.soundComponent.asset;
            deserialized.AddComponent<SoundComponent>(asset).sound = AssetsManager::GetInstance().GetSoundInstance(asset);
        }
        if (!entity.labelComponent.is_empty) {
            const auto& asset = entity.labelComponent.asset;
            const auto& content = entity.labelComponent.content;
            deserialized.AddComponent<LabelComponent>(asset).label = CreateRef<Label>(AssetsManager::GetInstance().GetFont(asset), content);
        }
        LOG_ENGINE_TRACE("Deserialized entity '{}' - name: {}", uuid, name);
    }
    return true;
}
}  // namespace de
