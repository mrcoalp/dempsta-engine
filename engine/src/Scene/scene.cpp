#include "Scene/scene.h"

#include "Renderer/renderer2d.h"
#include "Scene/entity.h"
#include "Scripting/scriptengine.h"

namespace de {
void Scene::OnUpdate(const TimeStep& ts) {
    auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
    for (const auto& entity : group) {
        const auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
        Renderer2D::DrawQuad((glm::mat4)transform, sprite.Color);
    }
    auto g = m_registry.view<ScriptComponent>();
    for (const auto& e : g) {
        auto& s = g.get<ScriptComponent>(e);
        SE::LoadFile(s.Path.c_str());
        if (!SE::CallFunction("OnUpdate", (float)ts)) {
            LOG_ENGINE_DEBUG("{} doesn't contain 'OnUpdate function", s.Path);
        }
    }
}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity(m_registry.create(), this);
    entity.AddComponent<TransformComponent>();  // Add transform by default
    entity.AddComponent<NameComponent>(name);
    return entity;
}
}  // namespace de
