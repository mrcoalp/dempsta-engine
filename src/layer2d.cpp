#include "layer2d.h"

#include <glm/gtc/type_ptr.hpp>

Layer2D::Layer2D() : Layer("2D"), m_cameraController(16.0f / 9.0f, true) {}

void Layer2D::OnAttach() {
    m_texture = de::Texture2D::Create("assets/textures/mask.png");
    m_dogTexture = de::Texture2D::Create("assets/textures/dog.jpg");
}

void Layer2D::OnDetach() {}

void Layer2D::OnUpdate(const de::TimeStep& ts) {
    static float xPosition = 0.0f;
    xPosition = fmod((xPosition += (float)ts * 2.0f), 5.0f);
    m_cameraController.OnUpdate(ts);
    de::RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});
    de::Renderer2D::ResetStatistics();
    de::Renderer2D::BeginScene(m_cameraController.GetCamera());

    for (float y = -5.0f; y < 5.0f; y += 0.5f) {
        for (float x = -5.0f; x < 5.0f; x += 0.5f) {
            de::Renderer2D::DrawQuad({x, y, -0.1f}, {0.3f, 0.3f}, {0.1f, 0.7f, 0.2f, 1.0f});
        }
    }

    de::Renderer2D::DrawQuad({xPosition, -0.5f, 0.0f}, {1.0f, 1.0f}, m_squareColor);

    de::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, m_texture, {1.0f, 0.0f, 1.0f, 1.0f});
    de::Renderer2D::DrawQuad({-1.0f, 0.0f}, {1.0f, 1.0f}, m_dogTexture);
    de::Renderer2D::DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, m_texture, {1.0f, 1.0f, 0.0f, 1.0f});
    de::Renderer2D::EndScene();
}

void Layer2D::OnEvent(de::Event& e) { m_cameraController.OnEvent(e); }

void Layer2D::OnImGuiRender() {
    ImGui::Begin("Square settings");
    ImGui::ColorEdit4("Color", glm::value_ptr(m_squareColor));
    ImGui::End();

    auto stats = de::Renderer2D::GetStatistics();
    ImGui::Begin("Renderer2D Statistics");
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Drawn Quads: %d", stats.quads);
    ImGui::Text("Drawn Vertices: %d", stats.GetDrawnVertices());
    ImGui::Text("Drawn Indices: %d", stats.GetDrawnIndices());
    ImGui::End();
}
