#include "gamelayer.h"

GameLayer::GameLayer() : de::Layer("GameLayer"), m_cameraController(16.0f / 9.0f) {}

void GameLayer::OnAttach() {
    m_spriteSheet = de::CreateRef<de::Atlas>("assets/textures/RPGpack_sheet_2X.png", glm::vec2({128.0f, 128.0f}));
    m_sprite = de::CreateRef<de::Sprite>(m_spriteSheet, glm::vec2({0.0f, 1.0f}), glm::vec2({1.0f, 2.0f}));
}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(const de::TimeStep& ts) {
    de::RenderCommand::Clear({0.4f, 0.4f, 0.2f, 1});
    de::Renderer2D::ResetStatistics();
    de::Renderer2D::BeginScene(m_cameraController.GetCamera());
    de::Renderer2D::DrawQuad({0.0f, 0.0f}, {0.5f, 1.0f}, m_sprite);
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
