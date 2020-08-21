#include "dempstaeditor.h"

#include <glm/gtc/type_ptr.hpp>

namespace de {
DempstaEditor::DempstaEditor() : Layer("DempstaEditor"), m_cameraController(16.0f / 9.0f, true) {}

void DempstaEditor::OnAttach() {
    m_spriteSheet = CreateRef<Atlas2D>("assets/textures/RPGpack_sheet_2X.png", glm::vec2(128.0f));
    m_spriteBarrel = SubTexture2D::CreateSprite(m_spriteSheet, glm::vec2({8.0f, 0.0f}));
    FrameBufferConfig fConfig = {1280, 720};
    m_frameBuffer = FrameBuffer::Create(fConfig);
    m_activeScene = CreateRef<Scene>();

    m_square = m_activeScene->CreateEntity("Square");
    m_square.AddComponent<SpriteComponent>();
    m_square.AddComponent<ScriptComponent>("assets/scripts/component_test.lua");

    auto camEntity = m_activeScene->CreateEntity("Primary Camera");
    camEntity.AddComponent<CameraComponent>().Primary = true;
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

    RenderCommand::Clear({0.4f, 0.4f, 0.2f, 1});

    m_activeScene->OnUpdate(ts);

    m_frameBuffer->Unbind();
}

void DempstaEditor::OnEvent(Event& e) {}

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

    if ((bool)m_square) {
        ImGui::Begin((const char*)m_square.GetComponent<NameComponent>());
        auto& color = m_square.GetComponent<SpriteComponent>().Color;
        ImGui::ColorEdit4("Select color", glm::value_ptr(color));
        ImGui::End();
    }

    ImGui::End();
}
}  // namespace de
