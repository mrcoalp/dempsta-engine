#include "dempsta.h"

class ExampleLayer : public de::Layer {
public:
    explicit ExampleLayer(const std::string& name) : Layer(name), m_camera(-3.2f, 3.2f, -1.8f, 1.8f) {
        m_vertexArray.reset(de::VertexArray::Create());

        float _vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.7f, 1.0f, 0.5f, -0.5f, 0.0f, 0.9f,
                                  0.7f,  0.0f,  1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.3f,  1.0f};

        std::shared_ptr<de::VertexBuffer> _vertexBuffer;
        _vertexBuffer.reset(de::VertexBuffer::Create(_vertices, sizeof(_vertices)));

        de::BufferLayout _layout = {{de::ShaderDataType::Vec3, "position"}, {de::ShaderDataType::Vec4, "color"}};
        _vertexBuffer->SetLayout(_layout);
        m_vertexArray->AddVertexBuffer(_vertexBuffer);

        uint32_t _indices[3] = {0, 1, 2};
        std::shared_ptr<de::IndexBuffer> _indexBuffer;
        _indexBuffer.reset(de::IndexBuffer::Create(_indices, sizeof(_indices) / sizeof(uint32_t)));
        m_vertexArray->AddIndexBuffer(_indexBuffer);

        std::string _vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 position;
            layout(location = 1) in vec4 color;

            uniform mat4 u_viewProjection;

            out vec3 o_position;
            out vec4 o_color;

            void main() {
                o_position = position;
                o_color = color;
                gl_Position = u_viewProjection * vec4(position, 1.0);
            }
        )";

        std::string _fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 o_position;
            in vec4 o_color;

            void main() {
                color = vec4(o_position * 0.5 + 0.5, 0.1);
                color = o_color;
            }
        )";

        m_shader = std::make_unique<de::Shader>(_vertexSrc, _fragmentSrc);

        m_squareVertexArray.reset(de::VertexArray::Create());

        float _sqaureVertices[3 * 4] = {-0.75f, -0.75f, 0.0f, 0.75f,  -0.75f, 0.0f,
                                        0.75f,  0.75f,  0.0f, -0.75f, 0.75f,  0.0f};

        _vertexBuffer.reset(de::VertexBuffer::Create(_sqaureVertices, sizeof(_sqaureVertices)));
        _layout = {{de::ShaderDataType::Vec3, "position"}};
        _vertexBuffer->SetLayout(_layout);
        m_squareVertexArray->AddVertexBuffer(_vertexBuffer);

        uint32_t _squareIndices[6] = {0, 1, 2, 2, 3, 0};
        _indexBuffer.reset(de::IndexBuffer::Create(_squareIndices, sizeof(_squareIndices) / sizeof(uint32_t)));
        m_squareVertexArray->AddIndexBuffer(_indexBuffer);

        std::string _vertexSquareSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 position;

            uniform mat4 u_viewProjection;

            void main() {
                gl_Position = u_viewProjection * vec4(position, 1.0);
            }
        )";

        std::string _fragmentSquareSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            void main() {
                color = vec4(0, 0, 0.2, 0.1);
            }
        )";

        m_squareShader = std::make_unique<de::Shader>(_vertexSquareSrc, _fragmentSquareSrc);
    }

    void OnUpdate() final {
        de::RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});
        de::Renderer::BeginScene(m_camera);
        de::Renderer::Submit(m_squareShader, m_squareVertexArray);
        de::Renderer::Submit(m_shader, m_vertexArray);
        de::Renderer::EndScene();
    }

    void OnEvent(de::Event& e) final {
        de::EventDispatcher _dispatcher(e);
        _dispatcher.Dispatch<de::KeyPressedEvent>(
            [this](de::KeyPressedEvent& event) { return OnKeyPressedEvent(event); });
    }

    bool OnKeyPressedEvent(de::KeyPressedEvent& event) {
        if (event.GetKeyCode() == DE_KEY_W) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(0.0f, 0.1f, 0.0f);
            m_camera.SetPosition(_position);
            return true;
        }
        if (event.GetKeyCode() == DE_KEY_S) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(0.0f, -0.1f, 0.0f);
            m_camera.SetPosition(_position);
            return true;
        }
        if (event.GetKeyCode() == DE_KEY_A) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(-0.1f, 0.0f, 0.0f);
            m_camera.SetPosition(_position);
            return true;
        }
        if (event.GetKeyCode() == DE_KEY_D) {
            glm::vec3 _position = m_camera.GetPosition() + glm::vec3(0.1f, 0.0f, 0.0f);
            m_camera.SetPosition(_position);
            return true;
        }
        return false;
    }

    void OnImGuiRender() final {
        ImGui::Begin("Debug Window");
        ImGui::Text("Client app debug ImGui window.");
        ImGui::End();
    }

private:
    std::shared_ptr<de::Shader> m_shader;
    std::shared_ptr<de::VertexArray> m_vertexArray;
    std::shared_ptr<de::Shader> m_squareShader;
    std::shared_ptr<de::VertexArray> m_squareVertexArray;
    de::OrthographicCamera m_camera;
};

class Game : public de::Application {
public:
    Game() { PushLayer(new ExampleLayer("belo exemplo")); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
