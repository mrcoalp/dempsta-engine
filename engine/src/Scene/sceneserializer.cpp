#include "Scene/sceneserializer.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Scene/components.h"
#include "Scene/entity.h"

using json = nlohmann::json;

namespace de {
template <typename Component>
static void AddComponentToJSON(Entity entity, const std::function<void(Component&)>& handleComponent) {
    if (entity.HasComponent<Component>()) {
        handleComponent(entity.GetComponent<Component>());
    }
}

void SceneSerializer::Serialize(const std::string& filePath) const {
    json entities;
    m_scene->m_registry.each([&](auto& entityId) {
        Entity entity = {entityId, m_scene.get()};
        json jsonEntity;
        jsonEntity["ID"] = "todo_uuid";
        AddComponentToJSON<NameComponent>(entity, [&jsonEntity](NameComponent& component) {
            jsonEntity["NameComponent"] = {{"Name", component.name}};
        });
        AddComponentToJSON<TransformComponent>(entity, [&jsonEntity](TransformComponent& component) {
            const auto& translation = component.translation;
            const auto& rotation = component.rotation;
            const auto& scale = component.scale;
            jsonEntity["TransformComponent"] = {{"Translation", {translation.x, translation.y, translation.z}},
                                                {"Rotation", {rotation.x, rotation.y, rotation.z}},
                                                {"Scale", {scale.x, scale.y, scale.z}}};
        });
        entities.push_back(jsonEntity);
    });
    json serialize;
    serialize["SomeSceneId"]["Entities"] = entities;
    std::ofstream out(filePath);
    out << serialize.dump(4) << std::endl;
}

bool SceneSerializer::Deserialize(const std::string& filePath) const { return true; }
}  // namespace de
