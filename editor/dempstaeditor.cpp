#include "dempstaeditor.h"

#include <glm/gtc/type_ptr.hpp>

DempstaEditor::DempstaEditor() : de::Layer("DempstaEditor"), m_cameraController(16.0f / 9.0f, true) {}

void DempstaEditor::OnAttach() {
    m_spriteSheet = de::CreateRef<de::Atlas2D>("assets/textures/RPGpack_sheet_2X.png", glm::vec2(128.0f));
    m_spriteBarrel = de::SubTexture2D::CreateSprite(m_spriteSheet, glm::vec2({8.0f, 0.0f}));
    de::FrameBufferConfig fConfig = {1280, 720};
    m_frameBuffer = de::FrameBuffer::Create(fConfig);
    m_activeScene = de::CreateRef<de::Scene>();

    m_square = m_activeScene->CreateEntity("Square");
    m_square.AddComponent<de::SpriteComponent>();
}

void DempstaEditor::OnDetach() {}

void DempstaEditor::OnUpdate(const de::TimeStep& ts) {
    m_ts = ts;
    m_timeAccumulator += (float)ts;
    // Update fps every half second
    if (m_timeAccumulator >= 0.5f) {
        m_timeAccumulator = 0.0f;
        m_fps = 1.0f / (float)ts;
    }
    if (m_editingMode) {
        m_cameraController.OnUpdate(ts);
    }

    if (m_editingMode) {
        m_frameBuffer->Bind();
    }

    de::RenderCommand::Clear({0.4f, 0.4f, 0.2f, 1});
    de::Renderer2D::ResetStatistics();

    static float rotation = 0.0f;
    rotation += 50.0f * (float)ts;

    de::Renderer2D::BeginScene(m_cameraController.GetCamera());

    m_activeScene->OnUpdate(ts);

    de::Renderer2D::DrawRotatedQuad(rotation, {-1.0f, 0.f}, {1.0f, 1.0f}, m_spriteBarrel);

    de::Renderer2D::EndScene();

    if (m_editingMode) {
        m_frameBuffer->Unbind();
    }
}

void DempstaEditor::OnEvent(de::Event& e) {
    de::EventDispatcher eventDispatcher(e);

    eventDispatcher.Dispatch<de::KeyPressedEvent>([this](de::KeyPressedEvent& event) {
        if (event.GetKeyCode() == DE_KEY_TAB) {
            m_editingMode = !m_editingMode;
            LOG_TRACE("Editing Mode: {}", m_editingMode);
            if (!m_editingMode) {
                const auto& window = de::Application::GetInstance().GetWindow();
                de::Renderer::OnWindowResize(window.GetWidth(), window.GetHeight());
                m_cameraController.OnResize(window.GetWidth(), window.GetHeight());
            }
        }
        return false;
    });

    m_cameraController.OnEvent(e);
}

void DempstaEditor::OnImGuiRender() {
    // Render only in editing mode
    if (!m_editingMode) {
        return;
    }

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
                de::Application::GetInstance().Close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    auto stats = de::Renderer2D::GetStatistics();
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
    float contentWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
    float contentHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;
    if ((uint32_t)contentHeight != m_frameBuffer->GetConfig().height ||
        (uint32_t)contentWidth != m_frameBuffer->GetConfig().width) {
        // Handle ImGui ViewPort resize
        m_frameBuffer->Resize((uint32_t)contentWidth, (uint32_t)contentHeight);
        de::Renderer::OnWindowResize((uint32_t)contentWidth, (uint32_t)contentHeight);
        m_cameraController.OnResize((uint32_t)contentWidth, (uint32_t)contentHeight);
    }

    ImGui::Image((void*)(uintptr_t)m_frameBuffer->GetColorAttachment(), {contentWidth, contentHeight}, {0, 1}, {1, 0});
    ImGui::End();

    if ((bool)m_square) {
        ImGui::Begin((const char*)m_square.GetComponent<de::NameComponent>());
        auto& color = m_square.GetComponent<de::SpriteComponent>().Color;
        ImGui::ColorEdit4("Select color", glm::value_ptr(color));
        ImGui::End();
    }

    ImGui::End();
}
