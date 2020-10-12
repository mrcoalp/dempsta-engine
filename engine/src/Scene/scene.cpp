#include "Scene/scene.h"

#include <unordered_map>

#include "Events/keyevent.h"
#include "Renderer/renderer2d.h"
#include "Scene/components.h"
#include "Scripting/messaging.h"

namespace de {
void Scene::OnUpdate(const TimeStep& ts) {
    // native scripting update
    m_registry.view<NativeScriptComponent>().each([&](const auto entity, auto& nsc) {
        if (nsc.instance == nullptr) {
            nsc.instance = nsc.Create();
            nsc.instance->m_entity = Entity(entity, this);
            nsc.instance->OnInit();
        }
        nsc.instance->OnUpdate(ts);
    });
    // scripting update
    m_registry.view<ScriptComponent>().each([&](const auto entity, auto& sc) {
        if (sc.instance == nullptr) {
            sc.Create();
            sc.instance->entityRef->m_entity = Entity(entity, this);
            sc.instance->ReloadScript();
        }
        sc.instance->OnUpdate(ts);
        // messaging
        lua::MessageHandler::HandleMessages([&sc](const lua::Message& msg) { sc.instance->OnMessage(msg.id, msg.data, msg.sender); });
    });
    lua::MessageHandler::ClearMessages();
    // sound
    m_registry.view<SoundComponent>().each([&]([[maybe_unused]] const auto soundEntt, const auto& soundComp) { soundComp.sound->Update(); });
    // render
    m_registry.view<TransformComponent, CameraComponent>().each(
        // cameraEntity can be avoided to be captured
        [&]([[maybe_unused]] const auto cameraEntity, const auto& transformComp, const auto& cameraComp) {
            const auto& camera = cameraComp.camera;
            const auto& transform = transformComp.transform;
            if (cameraComp.primary) {
                Renderer2D::BeginScene(camera.GetProjection(), transform);
                m_registry.view<TransformComponent, SpriteComponent>().each([](const auto& transformComp, const auto& spriteComp) {
                    if (spriteComp.texture != nullptr) {
                        Renderer2D::DrawQuad(transformComp.transform, spriteComp.texture, spriteComp.color);
                    } else {
                        Renderer2D::DrawQuad(transformComp.transform, spriteComp.color);
                    }
                });
                m_registry.view<TransformComponent, LabelComponent>().each([](const auto& transformComp, const auto& labelComp) {
                    Renderer2D::DrawQuad(transformComp.transform, labelComp.label, labelComp.color);
                });
                Renderer2D::EndScene();
                return;
            }
        });
}

void Scene::OnEvent(Event& event) {
    // native scripting update
    m_registry.view<NativeScriptComponent>().each([&](const auto entity, auto& nsc) { nsc.instance->OnEvent(event); });
    // scripting update
    EventDispatcher dispatcher(event);
    const int eventType = static_cast<int>(event.GetEventType());
    m_registry.view<ScriptComponent>().each([&](const auto entity, auto& sc) {
        if (!sc.instance->acquireEvents) {
            return;
        }
        dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& event) {
            sc.instance->OnEvent(eventType, event.GetKeyCode());
            return false;
        });
        dispatcher.Dispatch<MouseBtnPressedEvent>([&](MouseBtnPressedEvent& event) {
            sc.instance->OnEvent(eventType, event.GetMouseBtnCode());
            return false;
        });
    });
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_viewportWidth = width;
    m_viewportHeight = height;

    m_registry.view<CameraComponent>().each([&]([[maybe_unused]] const auto entity, auto& cameraComp) {
        if (!cameraComp.fixedAspectRatio) {
            cameraComp.camera.SetViewportSize(width, height);
        }
    });
}

Entity Scene::CreateEntity(const std::string& name, bool addTransform) {
    Entity entity(m_registry.create(), this);
    entity.AddComponent<NameComponent>(name);
    if (addTransform) {
        entity.AddComponent<TransformComponent>();  // Add transform by default
    }
    return entity;
}
}  // namespace de
