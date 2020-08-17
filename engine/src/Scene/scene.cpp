#include "Scene/scene.h"

#include "Renderer/renderer2d.h"
#include "Scene/entity.h"
#include "Scripting/scriptengine.h"

namespace de {
void Scene::OnUpdate(const TimeStep& ts) {
    m_registry.view<TransformComponent, CameraComponent>().each(
        // cameraEntity can be avoided to be captured
        [&]([[maybe_unused]] const auto cameraEntity, const auto& transformComp, const auto& cameraComp) {
            const auto& camera = cameraComp.Camera;
            const auto& transform = transformComp.Transform;
            if (cameraComp.Primary) {
                Renderer2D::BeginScene(camera.GetProjection(), transform);

                m_registry.group<TransformComponent>(entt::get<SpriteComponent, ScriptComponent>)
                    .each([ts](const auto& transformComp, const auto& spriteComp, const auto& scriptComp) {
                        Renderer2D::DrawQuad(transformComp.Transform, spriteComp.Color);
                        SE::LoadFile(scriptComp.Path.c_str());
                        SE::CallFunction("OnUpdate", scriptComp.Data.get(), (float)ts);
                    });
                Renderer2D::EndScene();
                return;
            }
        });
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_viewportWidth = width;
    m_viewportHeight = height;

    m_registry.view<CameraComponent>().each([&]([[maybe_unused]] const auto entity, auto& cameraComp) {
        if (!cameraComp.FixedAspectRatio) {
            cameraComp.Camera.SetViewportSize(width, height);
        }
    });
}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity(m_registry.create(), this);
    entity.AddComponent<TransformComponent>();  // Add transform by default
    entity.AddComponent<NameComponent>(name);
    return entity;
}
}  // namespace de
