#include "Scene/sceneserializer.h"

#include <nlohmann/json.hpp>

#include "Scene/components.h"
#include "Scene/entity.h"
#include "Utils/fileutils.h"

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
        auto entity = Entity(entityId, m_scene.get());
        json jEnt;
        jEnt["ID"] = 123;
        AddComponentToJSON<NameComponent>(entity, [&jEnt](NameComponent& component) { jEnt["NameComponent"]["Name"] = component.name; });
        AddComponentToJSON<TransformComponent>(entity, [&jEnt](TransformComponent& component) {
            jEnt["TransformComponent"]["Translation"] = {component.translation.x, component.translation.y, component.translation.z};
            jEnt["TransformComponent"]["Rotation"] = {component.rotation.x, component.rotation.y, component.rotation.z};
            jEnt["TransformComponent"]["Scale"] = {component.scale.x, component.scale.y, component.scale.z};
        });
        entities.push_back(jEnt);
    });
    json serialize;
    serialize["Scene"] = {{"ID", "todo_scene_name"}, {"Entities", entities}};
    FileUtils::WriteFile(filePath, serialize.dump(4));
}

bool SceneSerializer::Deserialize(const std::string& filePath) const {
    const auto jData = json::parse(FileUtils::ReadFile(filePath), nullptr, false);
    if (jData.is_discarded()) {
        LOG_ENGINE_ERROR("Ill formed JSON '{}'. Couldn't deserialize!", filePath);
        return false;
    }
    if (!jData.contains("Scene")) {
        LOG_ENGINE_ERROR("Provided JSON '{}' does not contain a required 'Scene' node! Couldn't deserialize!", filePath);
        return false;
    }
    auto sceneId = jData["Scene"]["ID"].get<std::string>();
    LOG_ENGINE_TRACE("Deserializing scene '{}'...", sceneId);
    if (jData["Scene"].contains("Entities")) {
        for (const auto& entity : jData["Scene"]["Entities"]) {
            const auto uuid = entity["ID"].get<uint32_t>();
            std::string name;
            if (entity.contains("NameComponent")) {
                name = entity["NameComponent"]["Name"].get<std::string>();
            }
            auto deserialized = m_scene->CreateEntity(name, false);
            if (entity.contains("TransformComponent")) {
                auto& component = deserialized.AddComponent<TransformComponent>();
                auto node = entity["TransformComponent"]["Translation"];
                component.translation = {node[0].get<float>(), node[1].get<float>(), node[2].get<float>()};
                node = entity["TransformComponent"]["Rotation"];
                component.rotation = {node[0].get<float>(), node[1].get<float>(), node[2].get<float>()};
                node = entity["TransformComponent"]["Scale"];
                component.scale = {node[0].get<float>(), node[1].get<float>(), node[2].get<float>()};
            }
            LOG_ENGINE_TRACE("Deserialized entity '{}' - name: {}", uuid, name);
        }
    }
    return true;
}
}  // namespace de
