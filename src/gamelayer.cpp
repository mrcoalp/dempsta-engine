#include "gamelayer.h"

GameLayer::GameLayer() : de::Layer("GameLayer"), m_cameraController(16.0f / 9.0f) {}

void GameLayer::OnAttach() {
    de::Application::GetInstance().GetWindow().SetVSync(false);
    m_spriteSheet = de::CreateRef<de::Atlas2D>("assets/textures/RPGpack_sheet_2X.png", glm::vec2({128.0f, 128.0f}));
    m_spriteTree = de::SubTexture2D::CreateSprite(m_spriteSheet, glm::vec2({0.0f, 1.0f}), glm::vec2({1.0f, 2.0f}));
    m_spriteBarrel = de::SubTexture2D::CreateSprite(m_spriteSheet, glm::vec2({8.0f, 0.0f}));
    de::FrameBufferConfig fConfig = {1280, 720};
    m_frameBuffer = de::FrameBuffer::Create(fConfig);
}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(const de::TimeStep& ts) {
    // LOG_DEBUG("{0} FPS", 1.0f / (float)ts);
    m_cameraController.OnUpdate(ts);
    m_frameBuffer->Bind();
    de::RenderCommand::Clear({0.4f, 0.4f, 0.2f, 1});
    de::Renderer2D::ResetStatistics();

    static float rotation = 0.0f;
    rotation += 50.0f * (float)ts;

    de::Renderer2D::BeginScene(m_cameraController.GetCamera());
    de::Renderer2D::DrawQuad({0.0f, 0.0f}, {0.5f, 1.0f}, m_spriteTree);

    m_quad.subTexture = m_spriteBarrel;
    m_quad.position = glm::vec3({0.5f, 0.0f, 0.0f});
    m_quad.size = glm::vec3({0.5f, 0.5f, 0.0f});
    m_quad.rotation = 45.0f;
    m_quad.tint = glm::vec4(1.0f, 0.1f, 0.2f, 0.7f);

    de::Renderer2D::DrawQuad(m_quad);
    de::Renderer2D::DrawRotatedQuad(rotation, {-1.0f, 0.f}, {1.0f, 1.0f}, m_spriteBarrel);

    de::Renderer2D::EndScene();
    m_frameBuffer->Unbind();
}

void GameLayer::OnEvent(de::Event& e) {
    de::EventDispatcher eventDispatcher(e);
    eventDispatcher.Dispatch<de::WindowResizeEvent>([this](de::WindowResizeEvent& event) {
        m_frameBuffer->Resize(event.GetWidth(), event.GetHeight());
        return false;
    });
    m_cameraController.OnEvent(e);
}

void GameLayer::OnImGuiRender() {
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

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool p_open = true;
    ImGui::Begin("Dempsta Editor", &p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen) ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
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
    ImGui::End();

    ImGui::Begin("ViewPort");
    float contentWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
    float contentHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;
    float fBufferWidth, fBufferHeight;

    if (m_frameBuffer->GetConfig().width >= m_frameBuffer->GetConfig().height) {
        fBufferWidth =
            contentWidth > m_frameBuffer->GetConfig().width ? m_frameBuffer->GetConfig().width : contentWidth;
        fBufferHeight = fBufferWidth / m_cameraController.GetAspectRatio();
    } else {
        fBufferHeight =
            contentHeight > m_frameBuffer->GetConfig().height ? m_frameBuffer->GetConfig().height : contentHeight;
        fBufferWidth = fBufferHeight / m_cameraController.GetAspectRatio();
    }

    ImGui::Image((void*)m_frameBuffer->GetColorAttachment(), {fBufferWidth, fBufferHeight}, {0, 1}, {1, 0});
    ImGui::End();

    ImGui::End();
}
