#include "layer2d.h"

#include <glm/gtc/type_ptr.hpp>

Layer2D::Layer2D() : Layer("2D"), m_cameraController(16.0f / 9.0f, true) {}

void Layer2D::OnAttach() { m_texture = de::Texture2D::Create("assets/textures/mask.png"); }

void Layer2D::OnDetach() {}

void Layer2D::OnUpdate(const de::TimeStep& ts) {
    m_cameraController.OnUpdate(ts);
    de::RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});
    de::Renderer2D::BeginScene(m_cameraController.GetCamera());
    de::Renderer2D::DrawQuad({-0.5f, 0.0f, -0.1f}, {0.6f, 0.6f}, m_squareColor);
    de::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.3f, 0.3f}, {0.1f, 0.7f, 0.2f, 1.0f});
    /*de::Renderer2D::DrawRotatedQuad(-20, {0.0f, 0.5f}, {0.2f, 0.2f}, m_texture, {1.0f, 0.0f, 0.0f, 1.0f});*/
    de::Renderer2D::EndScene();
}

void Layer2D::OnEvent(de::Event& e) { m_cameraController.OnEvent(e); }

void Layer2D::OnImGuiRender() {
    ImGui::Begin("Square settings");
    ImGui::ColorEdit4("Color", glm::value_ptr(m_squareColor));
    ImGui::End();
}
