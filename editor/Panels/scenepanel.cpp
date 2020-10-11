#include "scenepanel.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Scene/components.h"

namespace de {
static std::tuple<glm::vec3, glm::vec3, glm::vec3> GetTransformDecomposition(const glm::mat4& transform) {
    glm::vec3 scale;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::quat orientation;
    glm::decompose(transform, scale, orientation, translation, skew, perspective);
    glm::vec3 rotation = glm::degrees(glm::eulerAngles(orientation));

    return {translation, rotation, scale};
}

static void drawTransformNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
        ImGui::Spacing();
        auto& transform = entity.GetComponent<TransformComponent>().transform;
        auto [translate, rotation, scale] = GetTransformDecomposition(transform);
        bool dirty = false;
        if (ImGui::DragFloat3("Position", glm::value_ptr(translate), 0.1f)) {
            dirty = true;
        }
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f)) {
            dirty = true;
        }
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f)) {
            dirty = true;
        }

        if (dirty) {
            transform =
                glm::translate(glm::mat4(1.0f), translate) * glm::toMat4(glm::quat(glm::radians(rotation))) * glm::scale(glm::mat4(1.0f), scale);
        }

        ImGui::TreePop();
    }
}

static void drawScriptingNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Scripting")) {
        ImGui::Spacing();
        const auto& scriptEntity = entity.GetComponent<ScriptComponent>().instance;
        char buffer[128];
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, scriptEntity->GetPath().c_str(), scriptEntity->GetPath().length() + 1);
        if (ImGui::InputText("Script path", buffer, sizeof(buffer))) {
            scriptEntity->SetPath(std::string(buffer));
        }

        ImGui::Spacing();
        ImGui::SmallButton("Reload Script");
        if (ImGui::IsItemClicked()) {
            scriptEntity->ReloadScript();
        }

        auto [doubles, bools, strings] = scriptEntity->dataBuffer->GetData();
        for (auto& d : doubles) {
            auto d1 = static_cast<float>(d.second);
            if (ImGui::DragFloat(d.first.c_str(), &d1, 0.1f)) {
                d.second = d1;
            }
        }
        for (auto& b : bools) {
            ImGui::Checkbox(b.first.c_str(), &b.second);
        }
        for (auto& s : strings) {
            char* cstr = new char[s.second.length() + 1];
            memcpy(cstr, s.second.c_str(), s.second.length() + 1);
            if (ImGui::InputText(s.first.c_str(), cstr, 100)) {
                s.second = cstr;
            }
            delete[] cstr;
        }
        ImGui::TreePop();
    }
}

static void drawLabelNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(LabelComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Label")) {
        ImGui::Spacing();
        auto& label = entity.GetComponent<LabelComponent>();
        char buffer[128];
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, label.label->GetContent().c_str(), label.label->GetContent().length() + 1);
        if (ImGui::InputText("Content", buffer, sizeof(buffer))) {
            if (strlen(buffer) > 0) {
                label.label->SetContent(std::string(buffer));
            }
        }
        ImGui::ColorEdit4("Color", glm::value_ptr(label.color));

        ImGui::TreePop();
    }
}

static void drawSpriteNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(SpriteComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite")) {
        ImGui::Spacing();
        auto& sprite = entity.GetComponent<SpriteComponent>();
        ImGui::ColorEdit4("Color", glm::value_ptr(sprite.color));
        ImGui::DragFloat2("Anchor", glm::value_ptr(sprite.texture->GetAnchor()), 0.1f);

        ImGui::TreePop();
    }
}

static void drawSoundNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(SoundComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sound")) {
        ImGui::Spacing();
        const auto& sound = entity.GetComponent<SoundComponent>().sound;
        const auto& data = sound->GetAudioInfo();
        ImGui::Text("Channels: %d", data.channels);
        ImGui::Text("Sample Rate: %d", data.sampleRate);
        ImGui::Text("Bits Per Channel: %d", data.bitsPerSample);
        ImGui::Text("State: %s", sound->IsPlaying() ? "playing" : sound->IsPaused() ? "paused" : "stopped");
        ImGui::Spacing();
        if (ImGui::SmallButton("Play")) {
            if (sound->IsPaused()) {
                sound->Resume();
            } else {
                sound->Play();
            }
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Pause")) {
            sound->Pause();
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Stop")) {
            sound->Stop();
        }
        ImGui::Spacing();
        float gain = data.gain;
        float pitch = data.pitch;
        float pan = data.pan;
        bool looped = data.looped;
        if (ImGui::DragFloat("Gain", &gain, 0.05f, 0.f, 1.f)) {
            sound->SetGain(gain);
        }
        if (ImGui::DragFloat("Pitch", &pitch, 0.05f, 0.5f, 2.f)) {
            sound->SetPitch(pitch);
        }
        if (ImGui::DragFloat("Pan", &pan, 0.05f, -1.f, 1.f)) {
            sound->SetPan(pan);
        }
        if (ImGui::Checkbox("Looped", &looped)) {
            sound->SetLooped(looped);
        }
        ImGui::TreePop();
    }
}

static void drawComponents(Entity entity) {
    if (entity.HasComponent<NameComponent>()) {
        auto& name = entity.GetComponent<NameComponent>().name;
        char buffer[128];
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, name.c_str(), name.length() + 1);
        if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
            name = std::string(buffer);
        }
    }
    if (entity.HasComponent<TransformComponent>()) {
        drawTransformNode(entity);
        ImGui::Spacing();
    }
    if (entity.HasComponent<ScriptComponent>()) {
        drawScriptingNode(entity);
        ImGui::Spacing();
    }
    if (entity.HasComponent<LabelComponent>()) {
        drawLabelNode(entity);
        ImGui::Spacing();
    }
    if (entity.HasComponent<SpriteComponent>()) {
        drawSpriteNode(entity);
        ImGui::Spacing();
    }
    if (entity.HasComponent<SoundComponent>()) {
        drawSoundNode(entity);
        ImGui::Spacing();
    }
}

ScenePanel::ScenePanel(const Ref<Scene>& context) { SetContext(context); }

void ScenePanel::SetContext(const Ref<Scene>& context) { m_context = context; }

void ScenePanel::OnImGuiRender() {
    if (ImGui::Begin("Scene Hierarchy")) {
        m_context->m_registry.each([this](auto entityID) { drawEntityNode(Entity(entityID, m_context.get())); });

        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
            m_selected = {};
        }

        if (ImGui::BeginPopupContextWindow(nullptr, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                m_context->CreateEntity();
            }
            ImGui::EndPopup();
        }

        ImGui::End();

        if (ImGui::Begin("Properties")) {
            if (m_selected) {
                drawComponents(m_selected);

                if (ImGui::Button("Add Component")) {
                    ImGui::OpenPopup("AddComponentPanel");
                }

                if (ImGui::BeginPopup("AddComponentPanel")) {
                    if (!m_selected.HasComponent<CameraComponent>()) {
                        if (ImGui::Button("Camera")) {
                            m_selected.AddComponent<CameraComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    if (!m_selected.HasComponent<SpriteComponent>()) {
                        if (ImGui::Button("Sprite")) {
                            m_selected.AddComponent<SpriteComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    if (!m_selected.HasComponent<ScriptComponent>()) {
                        if (ImGui::Button("Script")) {
                            m_selected.AddComponent<ScriptComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
        }
    }
}

void ScenePanel::drawEntityNode(Entity entity) {
    ImGuiTreeNodeFlags flags =
        (entity == m_selected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    auto& name = entity.GetComponent<NameComponent>().name;
    bool open = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", name.c_str());
    if (ImGui::IsItemClicked()) {
        m_selected = entity;
    }
    if (open) {
        ImGui::TreePop();
    }
}
}  // namespace de
