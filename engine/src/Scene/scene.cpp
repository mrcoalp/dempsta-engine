#include "Scene/scene.h"

#include <unordered_map>

#include "Events/keyevent.h"
#include "Renderer/renderer2d.h"
#include "Scene/components.h"

namespace de {
void Scene::OnUpdate(const TimeStep& ts) {
    // native scripting update
    m_registry.view<NativeScriptComponent>().each([&](const auto entity, auto& nsc) {
        if (nsc.Instance == nullptr) {
            nsc.Instance = nsc.Create();
            nsc.Instance->m_entity = Entity(entity, this);
            nsc.Instance->OnInit();
        }
        nsc.Instance->OnUpdate(ts);
    });
    // scripting update
    m_registry.view<ScriptComponent>().each([&](const auto entity, auto& sc) {
        if (sc.EntityRef == nullptr) {
            sc.Data.reset(new lua::DataBuffer());
            sc.EntityRef.reset(new lua::LuaEntity());
            sc.EntityRef->m_entity = Entity(entity, this);
            sc.ReloadScript();
            sc.OnInit();
        }
        sc.OnUpdate(ts);
    });
    // render
    m_registry.view<TransformComponent, CameraComponent>().each(
        // cameraEntity can be avoided to be captured
        [&]([[maybe_unused]] const auto cameraEntity, const auto& transformComp, const auto& cameraComp) {
            const auto& camera = cameraComp.Camera;
            const auto& transform = transformComp.Transform;
            if (cameraComp.Primary) {
                Renderer2D::BeginScene(camera.GetProjection(), transform);

                m_registry.group<TransformComponent>(entt::get<SpriteComponent>)
                    .each([](const auto& transformComp, const auto& spriteComp) {
                        Renderer2D::DrawQuad(transformComp.Transform, spriteComp.Color);
                    });
                Renderer2D::EndScene();
                return;
            }
        });
}

void Scene::OnEvent(Event& event) {
    // native scripting update
    m_registry.view<NativeScriptComponent>().each([&](const auto entity, auto& nsc) { nsc.Instance->OnEvent(event); });
    // scripting update
    EventDispatcher dispatcher(event);
    const int eventType = static_cast<int>(event.GetEventType());
    m_registry.view<ScriptComponent>().each([&](const auto entity, auto& sc) {
        dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& event) {
            sc.OnEvent(eventType, event.GetKeyCode());
            return false;
        });
        dispatcher.Dispatch<MouseBtnPressedEvent>([&](MouseBtnPressedEvent& event) {
            sc.OnEvent(eventType, event.GetMouseBtnCode());
            return false;
        });
        dispatcher.Dispatch<MouseMovedEvent>([&](MouseMovedEvent& event) {
            std::unordered_map<std::string, float> coord = {{"x", event.GetX()}, {"y", event.GetY()}};
            sc.OnEvent(eventType, coord);
            return false;
        });
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
