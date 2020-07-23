#include "Scene/scene.h"

#include "Renderer/renderer2d.h"
#include "Scene/entity.h"

namespace de {
Scene::Scene() = default;

void Scene::OnUpdate(const TimeStep& ts) {
    auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
    for (const auto& entity : group) {
        const auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
        Renderer2D::DrawQuad((glm::mat4)transform, sprite.Color);
    }
}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity(m_registry.create(), this);
    entity.AddComponent<TransformComponent>();  // Add transform by default
    entity.AddComponent<NameComponent>(name);
    return entity;
}

}  // namespace de
