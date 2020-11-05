#include "Scene/sceneserializer.h"

#include "JSON/json.h"
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
    m_scene->ForEachEntity([&](const auto& entityId) {
        auto entity = Entity(entityId, m_scene.get());
        JSON::Entity jEntity;
        jEntity.id = 123;
        AddComponentToJSON<NameComponent>(entity, [&jEntity](NameComponent& component) { jEntity.nameComponent = {{}, true, component.name}; });
        AddComponentToJSON<TransformComponent>(entity, [&jEntity](TransformComponent& component) {
            JSON::Vec3 translation{component.translation};
            JSON::Vec3 rotation{component.rotation};
            JSON::Vec3 scale{component.scale};
            JSON::TransformComponent tc{{}, true, translation, rotation, scale};
            jEntity.transformComponent = tc;
        });
        AddComponentToJSON<CameraComponent>(entity, [&jEntity](CameraComponent& component) {
            JSON::CameraComponent cc{{}, true, component.primary, component.fixedAspectRatio};
            jEntity.cameraComponent = cc;
            //            jEnt["CameraComponent"]["Primary"] = component.primary;
            //            jEnt["CameraComponent"]["FixedAspectRatio"] = component.fixedAspectRatio;
            //            jEnt["CameraComponent"]["SceneCamera"]["Orthographic"] = {
            //                {"Size", component.camera.GetOrthographicSize()},
            //                {"NearClip", component.camera.GetOrthographicNearClip()},
            //                {"FarClip", component.camera.GetOrthographicFarClip()},
            //            };
        });
        AddComponentToJSON<SpriteComponent>(entity, [&jEntity](SpriteComponent& component) {
            const auto& color = component.color;
            //            jEnt["SpriteComponent"]["Color"] = {color.r, color.g, color.b, color.a};
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
    for (const auto& entity : jScene.entities) {
        auto uuid = entity.id;  // todo(mpinto): UUID
        std::string name;
        if (entity.nameComponent.added) {
            name = entity.nameComponent.name;
        }
        auto deserialized = m_scene->CreateEntity(name, false);
        if (entity.transformComponent.added) {
            const auto& saved = entity.transformComponent;
            auto& tc = deserialized.AddComponent<TransformComponent>();
            tc.translation = saved.translation.ToGLM();
            tc.rotation = saved.rotation.ToGLM();
            tc.scale = saved.scale.ToGLM();
        }
        if (entity.cameraComponent.added) {
            const auto& saved = entity.cameraComponent;
            auto& cc = deserialized.AddComponent<CameraComponent>();
            cc.primary = saved.primary;
            cc.fixedAspectRatio = saved.fixedAspectRatio;
        }
        LOG_ENGINE_TRACE("Deserialized entity '{}' - name: {}", uuid, name);
    }
    return true;
}
}  // namespace de
