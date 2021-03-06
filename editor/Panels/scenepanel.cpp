#include "scenepanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Core/assetsmanager.h"
#include "Core/math.h"
#include "Scene/components.h"

namespace de {
template <typename Component>
static void DrawComponent(const std::string& name, Entity entity, const std::function<void(Component&)>& uiFunction) {
    if (entity.HasComponent<Component>()) {
        bool open = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "%s", name.c_str());
        ImGui::SameLine(ImGui::GetWindowWidth() - 40.f);
        if (ImGui::SmallButton("...")) {
            ImGui::OpenPopup("Settings");
        }
        bool remove = false;
        if (ImGui::BeginPopup("Settings")) {
            if (ImGui::MenuItem("Remove component")) {
                remove = true;
            }
            ImGui::EndPopup();
        }
        if (open) {
            ImGui::Spacing();
            auto& component = entity.GetComponent<Component>();
            uiFunction(component);
            ImGui::TreePop();
        }
        if (remove) {
            entity.RemoveComponent<Component>();
        }
        ImGui::NewLine();
    }
}

static bool DrawVec3(const std::string& label, glm::vec3& values, float nameColWidth = 100.f, const glm::vec3& defaultValues = {0.f, 0.f, 0.f}) {
    ImGui::PushID(label.c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, nameColWidth);

    ImGui::Text("%s", label.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
    ImVec2 buttonSize = {lineHeight + 3.f, lineHeight};
    bool dirty = false;

    if (ImGui::Button("X", buttonSize)) {
        values.x = defaultValues.x;
        dirty = true;
    }
    ImGui::SameLine();
    if (ImGui::DragFloat("##x", &values.x, 0.1f)) {
        dirty = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Y", buttonSize)) {
        values.y = defaultValues.y;
        dirty = true;
    }
    ImGui::SameLine();
    if (ImGui::DragFloat("##y", &values.y, 0.1f)) {
        dirty = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Z", buttonSize)) {
        values.z = defaultValues.z;
        dirty = true;
    }
    ImGui::SameLine();
    if (ImGui::DragFloat("##z", &values.z, 0.1f)) {
        dirty = true;
    }
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns();
    ImGui::PopID();
    return dirty;
}

static void drawTransformNode(TransformComponent& component) {
    auto rotation = glm::degrees(component.rotation);  // For UI purposes, rotation in degrees makes more sense
    DrawVec3("Position", component.translation, 70.f);
    if (DrawVec3("Rotation", rotation, 70.f)) {
        component.rotation = glm::radians(rotation);
    }
    DrawVec3("Scale", component.scale, 70.f);
}

static void drawCameraNode(CameraComponent& component) {
    ImGui::Checkbox("Primary", &component.primary);
    ImGui::Checkbox("Fixed aspect ratio", &component.fixedAspectRatio);
}

template <class Component>
static void HandleAsset(Component& component, std::function<void(Component&, const char*)> reload) {
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, component.asset.c_str(), component.asset.length() + 1);
    if (ImGui::InputText("Asset", buffer, sizeof(buffer))) {
        ImGui::SameLine();
        ImGui::SmallButton("Reload");
        if (ImGui::IsItemClicked() && strlen(buffer) > 0) {
            reload(component, buffer);
        }
    }
}

static void drawScriptingNode(ScriptComponent& component) {
    HandleAsset<ScriptComponent>(component, [](ScriptComponent& component, const char* asset) {
        if (!AssetsManager::GetInstance().IsScript(asset)) {
            return;
        }
        component.asset = asset;
        component.instance->OnDestroy();
        component.instance.release();
    });
    if (component.instance == nullptr) {
        return;
    }
    if (ImGui::CollapsingHeader("Data")) {
        auto [doubles, bools, strings] = component.instance->dataBuffer->GetData();
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
    }
}

static void drawLabelNode(LabelComponent& component) {
    HandleAsset<LabelComponent>(component, [](LabelComponent& component, const char* asset) {
        if (!AssetsManager::GetInstance().IsFont(asset)) {
            return;
        }
        component.asset = asset;
        component.label = CreateRef<Label>(AssetsManager::GetInstance().GetFont(asset), "");
    });
    if (component.label == nullptr) {
        return;
    }
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, component.label->GetContent().c_str(), component.label->GetContent().length() + 1);
    if (ImGui::InputText("Content", buffer, sizeof(buffer))) {
        if (strlen(buffer) > 0) {
            component.label->SetContent(std::string(buffer));
        }
    }
    ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
}

static void drawSpriteNode(SpriteComponent& component) {
    HandleAsset<SpriteComponent>(component, [](SpriteComponent& component, const char* asset) { component.asset = asset; });
    ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
    ImGui::DragFloat2("Anchor", glm::value_ptr(component.anchor), 0.1f);
}

static void drawSoundNode(SoundComponent& component) {
    HandleAsset<SoundComponent>(component, [](SoundComponent& component, const char* asset) {
        if (!AssetsManager::GetInstance().IsSound(asset)) {
            return;
        }
        component.asset = asset;
        component.sound = AssetsManager::GetInstance().CreateSoundInstance(asset);
    });
    if (component.sound == nullptr) {
        return;
    }
    const auto& sound = component.sound;
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
}

template <typename Component>
static void AddComponent(
    const std::string& name, Entity entity, const std::function<void(Component&)>& onAdd = [](Component&) {}) {
    if (!entity.HasComponent<Component>()) {
        if (ImGui::Button(name.c_str())) {
            ImGui::CloseCurrentPopup();
            auto& component = entity.AddComponent<Component>();
            onAdd(component);
        }
    }
}

static void drawComponents(Entity entity, const Ref<Scene>& context) {
    if (entity.HasComponent<IDComponent>()) {
        ImGui::LabelText("UUID", "%lu", static_cast<uint64_t>(entity.GetComponent<IDComponent>().uuid));
    }
    if (entity.HasComponent<NameComponent>()) {
        auto& name = entity.GetComponent<NameComponent>().name;
        char buffer[128];
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, name.c_str(), name.length() + 1);
        if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
            name = std::string(buffer);
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("AddComponentPanel");
        }

        if (ImGui::BeginPopup("AddComponentPanel")) {
            AddComponent<TransformComponent>("Transform", entity);
            AddComponent<CameraComponent>("Camera", entity, [&](CameraComponent& component) { context->OnAddComponent(entity, component); });
            AddComponent<SpriteComponent>("Sprite", entity);
            AddComponent<ScriptComponent>("Script", entity);
            AddComponent<LabelComponent>("Label", entity);
            AddComponent<SoundComponent>("Sound", entity);
            ImGui::EndPopup();
        }
        ImGui::NewLine();
    }
    DrawComponent<TransformComponent>("Transform", entity, drawTransformNode);
    DrawComponent<CameraComponent>("Camera", entity, drawCameraNode);
    DrawComponent<ScriptComponent>("Scripting", entity, drawScriptingNode);
    DrawComponent<LabelComponent>("Label", entity, drawLabelNode);
    DrawComponent<SpriteComponent>("Sprite", entity, drawSpriteNode);
    DrawComponent<SoundComponent>("Sound", entity, drawSoundNode);
}

ScenePanel::ScenePanel(const Ref<Scene>& context) { SetContext(context); }

void ScenePanel::SetContext(const Ref<Scene>& context) { m_context = context; }

void ScenePanel::UnSelectEntity() { m_selected = {}; }

void ScenePanel::OnImGuiRender() {
    if (ImGui::Begin("Scene Hierarchy")) {
        m_context->ForEachEntity([this](const auto& entityId) { drawEntityNode(Entity(entityId, m_context.get())); });

        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
            UnSelectEntity();
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
                drawComponents(m_selected, m_context);
            }
            ImGui::End();
        }
    }
}

void ScenePanel::drawEntityNode(Entity entity) {
    ImGuiTreeNodeFlags flags = (entity == m_selected ? ImGuiTreeNodeFlags_Selected : 0);
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    auto& name = entity.GetComponent<NameComponent>().name;
    bool open = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", name.c_str());
    bool deleteEntity = false;
    if (ImGui::IsItemClicked()) {
        m_selected = entity;
    }
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete entity")) {
            deleteEntity = true;
        }
        ImGui::EndPopup();
    }
    if (deleteEntity) {
        if (m_selected == entity) {
            m_selected = {};
        }
        m_context->DestroyEntity(entity);
    }
    if (open) {
        ImGui::TreePop();
    }
}
}  // namespace de
