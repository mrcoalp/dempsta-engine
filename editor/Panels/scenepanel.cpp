#include "scenepanel.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

// #include <glm/gtx/matrix_decompose.hpp>

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

        ImGui::End();

        if (ImGui::Begin("Properties")) {
            if (m_selected) {
                drawComponents(m_selected);
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

void ScenePanel::drawTransformNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
        ImGui::Spacing();
        auto& transform = entity.GetComponent<TransformComponent>().transform;
        // glm::vec3 scale, translation, skew;
        // glm::vec4 perspective;
        // glm::quat orientation;
        // glm::decompose(transform, scale, orientation, translation, skew, perspective);
        ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
        ImGui::TreePop();
    }
}

void ScenePanel::drawScriptingNode(Entity entity) {
    if (ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Scripting")) {
        ImGui::Spacing();
        const auto& scriptEntity = entity.GetComponent<ScriptComponent>().instance;
        ImGui::SmallButton("Reload Script");
        ImGui::Spacing();
        if (ImGui::IsItemClicked()) {
            scriptEntity->ReloadScript();
        }
        auto [doubles, bools, strings] = scriptEntity->dataBuffer->GetData();
        for (auto& d : doubles) {
            auto d1 = static_cast<float>(d.second);
            ImGui::DragFloat(d.first.c_str(), &d1, 0.1f);
            d.second = d1;
        }
        for (auto& b : bools) {
            ImGui::Checkbox(b.first.c_str(), &b.second);
        }
        for (auto& s : strings) {
            char* cstr = new char[s.second.length() + 1];
            strcpy(cstr, s.second.c_str());
            ImGui::InputText(s.first.c_str(), cstr, 100);
            s.second = cstr;
            delete[] cstr;
        }
        ImGui::TreePop();
    }
}
}  // namespace de
