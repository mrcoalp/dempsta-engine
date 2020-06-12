#include "gamelayer.h"

GameLayer::GameLayer() : de::Layer("GameLayer"), m_cameraController(16.0f / 9.0f) {}

void GameLayer::OnAttach() {
    de::Application::GetInstance().GetWindow().SetVSync(false);
    m_spriteSheet = de::CreateRef<de::Atlas2D>("assets/textures/RPGpack_sheet_2X.png", glm::vec2({128.0f, 128.0f}));
    m_spriteTree = de::SubTexture2D::CreateSprite(m_spriteSheet, glm::vec2({0.0f, 1.0f}), glm::vec2({1.0f, 2.0f}));
    m_spriteBarrel = de::SubTexture2D::CreateSprite(m_spriteSheet, glm::vec2({8.0f, 0.0f}));
}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(const de::TimeStep& ts) {
    // LOG_DEBUG("{0} FPS", 1.0f / (float)ts);
    m_cameraController.OnUpdate(ts);
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
}

void GameLayer::OnEvent(de::Event& e) { m_cameraController.OnEvent(e); }

void GameLayer::OnImGuiRender() {
    auto stats = de::Renderer2D::GetStatistics();
    ImGui::Begin("Renderer2D Statistics");
    ImGui::Text("Max Quads Per Draw: %d", stats.maxQuadsPerDraw);
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Drawn Quads: %d", stats.quads);
    ImGui::Text("Drawn Vertices: %d", stats.GetDrawnVertices());
    ImGui::Text("Drawn Indices: %d", stats.GetDrawnIndices());
    ImGui::End();
}
