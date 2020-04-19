#include "dempsta.h"

class ExampleLayer : public de::Layer {
public:
    explicit ExampleLayer(const std::string& name) : Layer(name) {
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

            out vec3 o_position;
            out vec4 o_color;

            void main() {
                o_position = position;
                o_color = color;
                gl_Position = vec4(position, 1.0);
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
    }

    void OnUpdate() final {
        de::Renderer::BeginScene();
        de::RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});
        m_shader->Bind();
        de::Renderer::Submit(m_vertexArray);
        de::Renderer::EndScene();
    }

    void OnEvent(de::Event& e) final {
        // LOG_DEBUG("{0} {1}", GetName(), e.ToString());
        if (e.GetEventType() == de::EventType::KeyPressed) {
            auto& _event = (de::KeyEvent&)e;

            if (_event.GetKeyCode() == DE_KEY_SPACE) {
                LOG_DEBUG("Space PRESSED");
            }
        }
    }

    void OnImGuiRender() final {
        ImGui::Begin("Debug Window");
        ImGui::Text("Client app debug ImGui window.");
        ImGui::End();
    }

private:
    std::shared_ptr<de::Shader> m_shader;
    std::shared_ptr<de::VertexArray> m_vertexArray;
};

class Game : public de::Application {
public:
    Game() { PushLayer(new ExampleLayer("belo exemplo")); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
