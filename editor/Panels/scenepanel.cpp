#include "scenepanel.h"

#include <imgui.h>

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
    ImGuiTreeNodeFlags flags =
        (entity == m_selected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    auto& name = entity.GetComponent<NameComponent>().name;
    bool open = ImGui::TreeNodeEx((void*)(unsigned)entity, flags, "%s", name.c_str());
    if (ImGui::IsItemClicked()) {
        m_selected = entity;
    }
    if (open) {
        if (entity.HasComponent<TransformComponent>()) {
            drawTransformNode(entity);
        }
        if (entity.HasComponent<ScriptComponent>()) {
            drawScriptingNode(entity);
        }
        ImGui::TreePop();
    }
}

void ScenePanel::drawTransformNode(Entity entity) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    bool open = ImGui::TreeNodeEx("Position", flags);
    if (open) {
        if (ImGui::Begin("Edit")) {
            auto& transform = entity.GetComponent<TransformComponent>().transform;
            ImGui::SliderFloat("x", &transform[3][0], -20.f, 20.f);
            ImGui::SliderFloat("y", &transform[3][1], -20.f, 20.f);
            ImGui::SliderFloat("z", &transform[3][2], -20.f, 20.f);
            ImGui::End();
        }
        ImGui::TreePop();
    }
}

void ScenePanel::drawScriptingNode(Entity entity) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    bool open = ImGui::TreeNodeEx("Scripting", flags);
    if (open) {
        if (ImGui::Begin("Edit")) {
            const auto& scriptEntity = entity.GetComponent<ScriptComponent>().instance;
            auto [doubles, bools, strings] = scriptEntity->dataBuffer->GetData();
            for (auto& d : doubles) {
                auto d1 = static_cast<float>(d.second);
                ImGui::InputFloat(d.first.c_str(), &d1, 1.f, 10.f);
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
            ImGui::End();
        }
        ImGui::TreePop();
    }
}
}  // namespace de
