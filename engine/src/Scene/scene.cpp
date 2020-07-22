#include "Scene/scene.h"

#include "Renderer/renderer2d.h"

namespace de {
Scene::Scene() = default;

void Scene::OnUpdate(const TimeStep& ts) {
    auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
    for (const auto& entity : group) {
        const auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
        Renderer2D::DrawQuad((glm::mat4)transform, sprite.Color);
    }
}
}  // namespace de
