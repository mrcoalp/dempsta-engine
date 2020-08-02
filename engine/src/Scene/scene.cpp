#include "Scene/scene.h"

#include "Renderer/renderer2d.h"
#include "Scene/entity.h"
#include "Scripting/scriptengine.h"

namespace de {
void Scene::OnUpdate(const TimeStep& ts) {
    m_registry.view<TransformComponent, CameraComponent>().each(
        // cameraEntity can be avoided to be captured
        [&]([[maybe_unused]] const auto cameraEntity, const auto& transformComp, const auto& cameraComp) {
            const auto& camera = cameraComp.Cam;
            const auto& transform = transformComp.Transform;
            if (cameraComp.Primary) {
                Renderer2D::BeginScene(camera.GetProjection(), transform);

                m_registry.group<TransformComponent>(entt::get<SpriteComponent, ScriptComponent>)
                    .each([ts](const auto& transformComp, const auto& spriteComp, const auto& scriptComp) {
                        Renderer2D::DrawQuad(transformComp.Transform, spriteComp.Color);
                        SE::LoadFile(scriptComp.Path.c_str());
                        if (!SE::CallFunction("OnUpdate", (float)ts)) {
                            LOG_ENGINE_DEBUG("{} doesn't contain 'OnUpdate function", scriptComp.Path);
                        }
                    });
                Renderer2D::EndScene();
                return;
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
