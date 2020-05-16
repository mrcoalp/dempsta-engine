#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/openglshader.h"
#include "dempsta.h"

class ExampleLayer : public de::Layer {
public:
    explicit ExampleLayer(const std::string& name) : Layer(name), m_cameraController(16.0f / 9.0f, true) {
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

        // Load shaders
        m_shaderLib.Load("assets/shaders/triangle.glsl");
        m_shaderLib.Load("assets/shaders/square.glsl");
        auto _textureShader = m_shaderLib.Load("assets/shaders/texture.glsl");

        m_dogTexture = de::Texture2D::Create("assets/textures/dog.jpg");
        m_maskTexture = de::Texture2D::Create("assets/textures/mask.png");

        std::dynamic_pointer_cast<de::OpenGLShader>(_textureShader)->Bind();
        std::dynamic_pointer_cast<de::OpenGLShader>(_textureShader)->UploadUniformInt("u_texture", 0);
    }

    void OnUpdate(const de::TimeStep& ts) final {
        m_cameraController.OnUpdate(ts);

        de::RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});
        de::Renderer::BeginScene(m_cameraController.GetCamera());

        // Square
        auto _squareShader = m_shaderLib.Get("square");
        std::dynamic_pointer_cast<de::OpenGLShader>(_squareShader)->Bind();
        std::dynamic_pointer_cast<de::OpenGLShader>(_squareShader)->UploadUniformVec3("u_color", m_squareColor);
        de::Renderer::Submit(_squareShader, m_squareVertexArray);

        // Texture
        auto _textureShader = m_shaderLib.Get("texture");
        m_dogTexture->Bind();
        static const glm::mat4 _transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
        de::Renderer::Submit(_textureShader, m_squareVertexArray, _transform);

        m_maskTexture->Bind();
        static const glm::mat4 _transformMask = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 0.0f));
        de::Renderer::Submit(_textureShader, m_squareVertexArray,
                             _transformMask * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)));

        // Triangle
        de::Renderer::Submit(m_shaderLib.Get("triangle"), m_triangleVertexArray);
        de::Renderer::EndScene();
    }

    void OnEvent(de::Event& e) final {
        m_cameraController.OnEvent(e);
    }

    void OnImGuiRender() final {
        ImGui::Begin("Square settings");
        ImGui::ColorEdit3("Color", glm::value_ptr(m_squareColor));
        ImGui::End();
    }

private:
    de::ShaderLibrary m_shaderLib;
    de::Ref<de::VertexArray> m_triangleVertexArray;
    de::Ref<de::VertexArray> m_squareVertexArray;
    de::OrthographicCameraController m_cameraController;
    glm::vec3 m_squareColor{0.0f, 0.0f, 0.2f};
    de::Ref<de::Texture2D> m_dogTexture, m_maskTexture;
};

class Game : public de::Application {
public:
    Game() { PushLayer(new ExampleLayer("belo exemplo")); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
