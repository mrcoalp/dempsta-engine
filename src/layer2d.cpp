#include "layer2d.h"

#include <Platform/OpenGL/openglshader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Layer2D::Layer2D() : Layer("2D"), m_cameraController(16.0f / 9.0f, true) {}

void Layer2D::OnAttach() {
    m_squareVertexArray = de::VertexArray::Create();

    float _squareVertices[3 * 4] = {
        -0.75f, -0.75f, 0.0f,
        0.75f,  -0.75f, 0.0f,
        0.75f,  0.75f,  0.0f,
        -0.75f, 0.75f,  0.0f
    };

    auto _vertexBuffer = de::VertexBuffer::Create(_squareVertices, sizeof(_squareVertices));
    de::BufferLayout _layout = {{de::ShaderDataType::Vec3, "position"}};
    _vertexBuffer->SetLayout(_layout);
    m_squareVertexArray->AddVertexBuffer(_vertexBuffer);

    unsigned _squareIndices[6] = {0, 1, 2, 2, 3, 0};
    auto _indexBuffer = de::IndexBuffer::Create(_squareIndices, sizeof(_squareIndices) / sizeof(unsigned));
    m_squareVertexArray->AddIndexBuffer(_indexBuffer);

    m_squareShader = de::Shader::Create("assets/shaders/square.glsl");
}

void Layer2D::OnDetach() {}

void Layer2D::OnUpdate(const de::TimeStep& ts) {
    m_cameraController.OnUpdate(ts);

    de::RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});
    de::Renderer::BeginScene(m_cameraController.GetCamera());

    // Square
    std::dynamic_pointer_cast<de::OpenGLShader>(m_squareShader)->Bind();
    std::dynamic_pointer_cast<de::OpenGLShader>(m_squareShader)->UploadUniformVec4("u_color", m_squareColor);
    de::Renderer::Submit(m_squareShader, m_squareVertexArray);

    de::Renderer::EndScene();
}

void Layer2D::OnEvent(de::Event& e) { m_cameraController.OnEvent(e); }

void Layer2D::OnImGuiRender() {
    ImGui::Begin("Square settings");
    ImGui::ColorEdit4("Color", glm::value_ptr(m_squareColor));
    ImGui::End();
}
