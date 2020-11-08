#include "Scene/scene.h"

#include <unordered_map>

#include "Core/assetsmanager.h"
#include "Events/keyevent.h"
#include "Renderer/renderer2d.h"
#include "Scene/components.h"
#include "Scripting/messaging.h"

namespace de {
void Scene::OnUpdate(const TimeStep& ts) {
    // native scripting update
    m_registry.view<NativeScriptComponent>().each([&](const auto entity, NativeScriptComponent& nsc) {
        if (nsc.instance == nullptr) {
            nsc.instance = nsc.Create();
            nsc.instance->m_entity = Entity(entity, this);
            nsc.instance->OnInit();
        }
        nsc.instance->OnUpdate(ts);
    });
    // scripting update
    m_registry.view<ScriptComponent>().each([&](const auto entity, ScriptComponent& sc) {
        if (!de::AssetsManager::GetInstance().IsScript(sc.asset)) {
            return;
        }
        if (sc.instance == nullptr) {
            sc.instance = CreateScope<lua::ScriptEntity>(AssetsManager::GetInstance().GetScript(sc.asset));
            sc.instance->entityRef->m_entity = Entity(entity, this);
            sc.instance->OnInit();
        }
        sc.instance->OnUpdate(ts);
        // messaging
        lua::MessageHandler::HandleMessages([&sc](const lua::Message& msg) { sc.instance->OnMessage(msg.id, msg.sender, msg.data); });
    });
    lua::MessageHandler::ClearMessages();
    // sound
    m_registry.view<SoundComponent>().each([&]([[maybe_unused]] const auto soundEntt, const SoundComponent& soundComp) {
        if (soundComp.sound != nullptr) {
            soundComp.sound->Update();
        }
    });
    // render
    m_registry.view<TransformComponent, CameraComponent>().each(
        // cameraEntity can be avoided to be captured
        [&]([[maybe_unused]] const auto cameraEntity, const auto& transformComp, const auto& cameraComp) {
            const auto& camera = cameraComp.camera;
            const auto& transform = transformComp.GetTransform();
            if (cameraComp.primary) {
                Renderer2D::BeginScene(camera.GetProjection(), transform);
                m_registry.view<TransformComponent, SpriteComponent>().each([](const auto& transformComp, const auto& spriteComp) {
                    const auto& assets = AssetsManager::GetInstance();
                    if (assets.IsSprite(spriteComp.asset)) {
                        const auto& sprite = assets.GetSprite(spriteComp.asset);
                        Renderer2D::DrawQuad(transformComp.GetTransform(), sprite, spriteComp.color, spriteComp.anchor);
                    } else {
                        Renderer2D::DrawQuad(transformComp.GetTransform(), spriteComp.color, spriteComp.anchor);
                    }
                });
                m_registry.view<TransformComponent, LabelComponent>().each([](const auto& transformComp, const auto& labelComp) {
                    if (labelComp.label == nullptr) {
                        return;
                    }
                    Renderer2D::DrawQuad(transformComp.GetTransform(), labelComp.label, labelComp.color);
                });
                Renderer2D::EndScene();
                return;
            }
        });
}

void Scene::OnEvent(Event& event) {
    // native scripting update
    m_registry.view<NativeScriptComponent>().each([&](const auto entity, NativeScriptComponent& nsc) { nsc.instance->OnEvent(event); });
    // scripting update
    EventDispatcher dispatcher(event);
    const int eventType = static_cast<int>(event.GetEventType());
    m_registry.view<ScriptComponent>().each([&](const auto entity, ScriptComponent& sc) {
        if (sc.instance == nullptr) {
            return;
        }
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

    m_registry.view<CameraComponent>().each([&]([[maybe_unused]] const auto entity, CameraComponent& cameraComp) {
        if (!cameraComp.fixedAspectRatio) {
            cameraComp.camera.SetViewportSize(width, height);
        }
    });
}

template <typename Component>
void Scene::OnAddComponent([[maybe_unused]] Entity entity, Component& component) {
    DE_THROW("Invalid component")
}

template <>
void Scene::OnAddComponent<NameComponent>([[maybe_unused]] Entity entity, NameComponent& component) {}

template <>
void Scene::OnAddComponent<TransformComponent>([[maybe_unused]] Entity entity, TransformComponent& component) {}

template <>
void Scene::OnAddComponent<SpriteComponent>([[maybe_unused]] Entity entity, SpriteComponent& component) {}

template <>
void Scene::OnAddComponent<ScriptComponent>([[maybe_unused]] Entity entity, ScriptComponent& component) {}

template <>
void Scene::OnAddComponent<NativeScriptComponent>([[maybe_unused]] Entity entity, NativeScriptComponent& component) {}

template <>
void Scene::OnAddComponent<CameraComponent>([[maybe_unused]] Entity entity, CameraComponent& component) {
    component.camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
}

template <>
void Scene::OnAddComponent<LabelComponent>([[maybe_unused]] Entity entity, LabelComponent& component) {}

template <>
void Scene::OnAddComponent<SoundComponent>([[maybe_unused]] Entity entity, SoundComponent& component) {}

Entity Scene::CreateEntity(const std::string& name, bool addTransform) {
    Entity entity(m_registry.create(), this);
    entity.AddComponent<NameComponent>(name);
    if (addTransform) {
        entity.AddComponent<TransformComponent>();  // Add transform by default
    }
    return entity;
}

void Scene::DestroyEntity(Entity entity) { m_registry.destroy((entt::entity)entity); }
}  // namespace de
