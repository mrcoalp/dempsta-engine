#include "scenepanel.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Scene/components.h"

namespace de {
ScenePanel::ScenePanel(const Ref<Scene>& context) { SetContext(context); }

void ScenePanel::SetContext(const Ref<Scene>& context) { m_context = context; }

void ScenePanel::OnImGuiRender() {
    if (ImGui::Begin("Scene Hierarchy")) {
        m_context->m_registry.each([this](auto entityID) { drawEntityNode(Entity(entityID, m_context.get())); });
        ImGui::End();
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
        if (ImGui::Begin("Properties")) {
            if (entity.HasComponent<TransformComponent>()) {
                drawTransformNode(entity);
                ImGui::Spacing();
            }
            if (entity.HasComponent<ScriptComponent>()) {
                drawScriptingNode(entity);
                ImGui::Spacing();
            }
            ImGui::End();
        }
        ImGui::TreePop();
    }
}

void ScenePanel::drawTransformNode(Entity entity) {
    if (ImGui::CollapsingHeader("Position", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Spacing();
        auto& transform = entity.GetComponent<TransformComponent>().transform;
        // glm::vec3 scale, translation, skew;
        // glm::vec4 perspective;
        // glm::quat orientation;
        // glm::decompose(transform, scale, orientation, translation, skew, perspective);
        ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
    }
}

void ScenePanel::drawScriptingNode(Entity entity) {
    if (ImGui::CollapsingHeader("Scripting", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
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
            mempcpy(cstr, s.second.c_str(), s.second.length() + 1);
            ImGui::InputText(s.first.c_str(), cstr, 100);
            s.second = cstr;
            delete[] cstr;
        }
    }
}
}  // namespace de
