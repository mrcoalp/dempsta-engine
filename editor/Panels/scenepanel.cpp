#include "scenepanel.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Scene/components.h"

namespace de {
ScenePanel::ScenePanel(const Ref<Scene>& context) { SetContext(context); }

void ScenePanel::SetContext(const Ref<Scene>& context) { m_context = context; }

void ScenePanel::OnImGuiRender() {
    if (ImGui::Begin("Scene Hierarchy")) {
        m_context->m_registry.each([this](auto entityID) { drawEntityNode(Entity(entityID, m_context.get())); });

        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
            m_selected = {};
        }

        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
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
    ImGuiTreeNodeFlags flags = (entity == m_selected ? ImGuiTreeNodeFlags_Selected : 0) |
                               ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    auto& name = entity.GetComponent<NameComponent>().name;
    bool open = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", name.c_str());
    if (ImGui::IsItemClicked()) {
        m_selected = entity;
    }
    if (open) {
        ImGui::TreePop();
    }
}

void ScenePanel::drawComponents(Entity entity) {
    if (entity.HasComponent<NameComponent>()) {
        auto& name = entity.GetComponent<NameComponent>().name;
        char buffer[128];
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, name.c_str(), name.length());
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
}

static std::tuple<glm::vec3, glm::vec3, glm::vec3> GetTransformDecomposition(const glm::mat4& transform) {
    glm::vec3 scale, translation, skew;
    glm::vec4 perspective;
    glm::quat orientation;
    glm::decompose(transform, scale, orientation, translation, skew, perspective);
    glm::vec3 rotation = glm::degrees(glm::eulerAngles(orientation));

    return {translation, rotation, scale};
}

void ScenePanel::drawTransformNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
        ImGui::Spacing();
        auto& transform = entity.GetComponent<TransformComponent>().transform;
        auto [translate, rotation, scale] = GetTransformDecomposition(transform);
        bool dirty = false;
        if (ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f)) {
            dirty = true;
        }
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.25f)) {
            dirty = true;
        }
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f)) {
            dirty = true;
        }

        if (dirty) {
            transform = glm::translate(glm::mat4(1.0f), translate) * glm::toMat4(glm::quat(glm::radians(rotation))) *
                        glm::scale(glm::mat4(1.0f), scale);
        }

        ImGui::TreePop();
    }
}

void ScenePanel::drawScriptingNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Scripting")) {
        ImGui::Spacing();
        const auto& scriptEntity = entity.GetComponent<ScriptComponent>().instance;
        char buffer[128];
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, scriptEntity->GetPath().c_str(), scriptEntity->GetPath().length());
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
            strcpy(cstr, s.second.c_str());
            if (ImGui::InputText(s.first.c_str(), cstr, 100)) {
                s.second = cstr;
            }
            delete[] cstr;
        }
        ImGui::TreePop();
    }
}
}  // namespace de
