#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/openglshader.h"
#include "dempsta.h"

class ExampleLayer : public de::Layer {
public:
    explicit ExampleLayer(const std::string& name) : Layer(name), m_camera(-3.2f, 3.2f, -1.8f, 1.8f) {
        m_triangleVertexArray = de::VertexArray::Create();

        float _vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.7f, 1.0f, 0.5f, -0.5f, 0.0f, 0.9f,
                                  0.7f,  0.0f,  1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.3f,  1.0f};

        de::Ref<de::VertexBuffer> _vertexBuffer = de::VertexBuffer::Create(_vertices, sizeof(_vertices));

        de::BufferLayout _layout = {{de::ShaderDataType::Vec3, "position"}, {de::ShaderDataType::Vec4, "color"}};
        _vertexBuffer->SetLayout(_layout);
        m_triangleVertexArray->AddVertexBuffer(_vertexBuffer);

        unsigned _indices[3] = {0, 1, 2};
        de::Ref<de::IndexBuffer> _indexBuffer = de::IndexBuffer::Create(_indices, sizeof(_indices) / sizeof(unsigned));
        m_triangleVertexArray->AddIndexBuffer(_indexBuffer);

        m_triangleShader = de::Shader::Create("assets/shaders/triangle.glsl");

        m_squareVertexArray = de::VertexArray::Create();

        float _squareVertices[5 * 4] = {
            -0.75f, -0.75f, 0.0f, 0.0f, 0.0f, 0.75f,  -0.75f, 0.0f, 1.0f, 0.0f,
            0.75f,  0.75f,  0.0f, 1.0f, 1.0f, -0.75f, 0.75f,  0.0f, 0.0f, 1.0f,
        };

        _vertexBuffer = de::VertexBuffer::Create(_squareVertices, sizeof(_squareVertices));
        _layout = {{de::ShaderDataType::Vec3, "position"}, {de::ShaderDataType::Vec2, "texture"}};
        _vertexBuffer->SetLayout(_layout);
        m_squareVertexArray->AddVertexBuffer(_vertexBuffer);

        unsigned _squareIndices[6] = {0, 1, 2, 2, 3, 0};
        _indexBuffer = de::IndexBuffer::Create(_squareIndices, sizeof(_squareIndices) / sizeof(unsigned));
        m_squareVertexArray->AddIndexBuffer(_indexBuffer);

        m_squareShader = de::Shader::Create("assets/shaders/square.glsl");

        m_textureShader = de::Shader::Create("assets/shaders/texture.glsl");

        m_dogTexture = de::Texture2D::Create("assets/textures/dog.jpg");
        m_maskTexture = de::Texture2D::Create("assets/textures/mask.png");

        std::dynamic_pointer_cast<de::OpenGLShader>(m_textureShader)->Bind();
        std::dynamic_pointer_cast<de::OpenGLShader>(m_textureShader)->UploadUniformInt("u_texture", 0);
    }

    void OnUpdate(const de::TimeStep& ts) final {
        if (de::Input::IsKeyPressed(DE_KEY_W)) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(0.0f, 1.0f * (float)ts, 0.0f);
            m_camera.SetPosition(_position);
        }
        if (de::Input::IsKeyPressed(DE_KEY_S)) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(0.0f, -1.0f * (float)ts, 0.0f);
            m_camera.SetPosition(_position);
        }
        if (de::Input::IsKeyPressed(DE_KEY_A)) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(-1.0f * (float)ts, 0.0f, 0.0f);
            m_camera.SetPosition(_position);
        }
        if (de::Input::IsKeyPressed(DE_KEY_D)) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(1.0f * (float)ts, 0.0f, 0.0f);
            m_camera.SetPosition(_position);
        }

        de::RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});
        de::Renderer::BeginScene(m_camera);

        // Square
        std::dynamic_pointer_cast<de::OpenGLShader>(m_squareShader)->Bind();
        std::dynamic_pointer_cast<de::OpenGLShader>(m_squareShader)->UploadUniformVec3("u_color", m_squareColor);
        de::Renderer::Submit(m_squareShader, m_squareVertexArray);

        // Texture
        m_dogTexture->Bind();
        static const glm::mat4 _transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
        de::Renderer::Submit(m_textureShader, m_squareVertexArray, _transform);

        m_maskTexture->Bind();
        static const glm::mat4 _transformMask = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 0.0f));
        de::Renderer::Submit(m_textureShader, m_squareVertexArray,
                             _transformMask * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)));

        // Triangle
        de::Renderer::Submit(m_triangleShader, m_triangleVertexArray);
        de::Renderer::EndScene();
    }

    void OnEvent(de::Event& e) final {}

    void OnImGuiRender() final {
        ImGui::Begin("Square settings");
        ImGui::ColorEdit3("Color", glm::value_ptr(m_squareColor));
        ImGui::End();
    }

private:
    de::Ref<de::Shader> m_triangleShader, m_squareShader, m_textureShader;
    de::Ref<de::VertexArray> m_triangleVertexArray;
    de::Ref<de::VertexArray> m_squareVertexArray;
    de::OrthographicCamera m_camera;
    glm::vec3 m_squareColor{0.0f, 0.0f, 0.2f};
    de::Ref<de::Texture2D> m_dogTexture, m_maskTexture;
};

class Game : public de::Application {
public:
    Game() { PushLayer(new ExampleLayer("belo exemplo")); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
