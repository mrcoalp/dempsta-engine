#include "dempstaeditor.h"

#include <glm/gtc/type_ptr.hpp>

namespace de {
DempstaEditor::DempstaEditor() : Layer("DempstaEditor") {}

void DempstaEditor::OnAttach() {
    FrameBufferConfig fConfig = {1280, 720};
    m_frameBuffer = FrameBuffer::Create(fConfig);
    m_activeScene = CreateRef<Scene>();

    auto spriteSheet = CreateRef<Atlas2D>("assets/textures/RPGpack_sheet_2X.png", glm::vec2(128.0f));
    auto spriteBarrel = SubTexture2D::CreateSprite(spriteSheet, glm::vec2({8.0f, 0.0f}));
    auto spriteDog = SubTexture2D::CreateSprite("assets/textures/dog.jpg");
    auto spriteMask = SubTexture2D::CreateSprite("assets/textures/mask.png");
    auto spriteSphere = SubTexture2D::CreateSprite("assets/textures/sphere.png");
    auto text = CreateRef<Label>("assets/fonts/arial.ttf", 1);
    text->SetContent("Hello world");

    // m_sphere = m_activeScene->CreateEntity("Sphere");
    // m_sphere.AddComponent<SpriteComponent>().texture = spriteSphere;
    // m_sphere.AddComponent<ScriptComponent>("assets/scripts/sphere.lua");

    // for (size_t i = 0; i < 5; ++i) {
    //     auto mask = m_activeScene->CreateEntity("Mask_" + std::to_string(i));
    //     mask.AddComponent<SpriteComponent>().texture = spriteMask;
    //     mask.AddComponent<ScriptComponent>("assets/scripts/mask.lua");
    // }

    auto textEnt = m_activeScene->CreateEntity("TestText");
    textEnt.AddComponent<LabelComponent>().label = text;

    auto camEntity = m_activeScene->CreateEntity("Primary Camera");
    camEntity.AddComponent<CameraComponent>().primary = true;
    camEntity.AddComponent<ScriptComponent>("assets/scripts/camera.lua");
}

void DempstaEditor::OnDetach() {}

void DempstaEditor::OnUpdate(const TimeStep& ts) {
    m_ts = ts;
    m_timeAccumulator += (float)ts;
    // Update fps every half second
    if (m_timeAccumulator >= 0.5f) {
        m_timeAccumulator = 0.0f;
        m_fps = 1.0f / (float)ts;
    }

    auto fConfig = m_frameBuffer->GetConfig();

    if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
        (fConfig.width != m_viewportSize.x || fConfig.height != m_viewportSize.y)) {
        const auto width = (uint32_t)m_viewportSize.x;
        const auto height = (uint32_t)m_viewportSize.y;
        m_frameBuffer->Resize(width, height);
        m_activeScene->OnViewportResize(width, height);
    }

    Renderer2D::ResetStatistics();

    m_frameBuffer->Bind();

    RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});

    m_activeScene->OnUpdate(ts);

    m_frameBuffer->Unbind();
}

void DempstaEditor::OnEvent(Event& e) { m_activeScene->OnEvent(e); }

void DempstaEditor::OnImGuiRender() {
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if ((dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool p_open = true;
    ImGui::Begin("Dempsta Editor", &p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen) {
        ImGui::PopStyleVar(2);
    }

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // ImGui::Separator();
            if (ImGui::MenuItem("Quit", nullptr, false, true)) {
                Application::GetInstance().Close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    auto stats = Renderer2D::GetStatistics();
    ImGui::Begin("Renderer2D Statistics");
    ImGui::Text("Max Quads Per Draw: %d", stats.maxQuadsPerDraw);
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Drawn Quads: %d", stats.quads);
    ImGui::Text("Drawn Vertices: %d", stats.GetDrawnVertices());
    ImGui::Text("Drawn Indices: %d", stats.GetDrawnIndices());
    ImGui::Text("Time Per Frame: %.6f", (float)m_ts);
    ImGui::Text("FPS: %.0f", m_fps);
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100.0f, 100.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("ViewPort", &p_open, ImGuiWindowFlags_NoCollapse);
    ImGui::PopStyleVar(2);
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(uintptr_t)m_frameBuffer->GetColorAttachment(), viewportPanelSize, {0, 1}, {1, 0});
    m_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
    ImGui::End();

    if ((bool)m_sphere) {
        ImGui::Begin((const char*)m_sphere.GetComponent<NameComponent>());
        auto& color = m_sphere.GetComponent<SpriteComponent>().color;
        ImGui::ColorEdit4("Select color", glm::value_ptr(color));
        ImGui::End();
    }

    ImGui::End();
}
}  // namespace de
