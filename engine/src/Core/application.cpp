#include "Core/application.h"

#include <glad/glad.h>

#include "Core/core.h"
#include "Core/log.h"
#include "Input/input.h"

namespace de {
Application* Application::m_instance = nullptr;

Application::Application() {
    DE_ASSERT(m_instance == nullptr, "Application already exists! Aborting...")

    m_instance = this;

    m_window = std::make_unique<Window>(WindowProps());
    m_window->SetEventCallback([this](Event& e) { OnEvent(e); });

    m_imguiLayer = new ImGuiLayer();
    PushOverlay(m_imguiLayer);

    m_vertexArray.reset(VertexArray::Create());

    float _vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.7f, 1.0f, 0.5f, -0.5f, 0.0f, 0.9f,
                              0.7f,  0.0f,  1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.3f,  1.0f};

    std::shared_ptr<VertexBuffer> _vertexBuffer;
    _vertexBuffer.reset(VertexBuffer::Create(_vertices, sizeof(_vertices)));

    BufferLayout _layout = {{ShaderDataType::Vec3, "position"}, {ShaderDataType::Vec4, "color"}};
    _vertexBuffer->SetLayout(_layout);
    m_vertexArray->AddVertexBuffer(_vertexBuffer);

    uint32_t _indices[3] = {0, 1, 2};
    std::shared_ptr<IndexBuffer> _indexBuffer;
    _indexBuffer.reset(IndexBuffer::Create(_indices, sizeof(_indices) / sizeof(uint32_t)));
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

    m_shader = std::make_unique<Shader>(_vertexSrc, _fragmentSrc);
}

Application::~Application() = default;

void Application::OnEvent(Event& e) {
    EventDispatcher _eventDispatcher(e);
    _eventDispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return onWindowClose(event); });
    // Handle layer events
    for (auto _it = m_layerStack.rbegin(); _it != m_layerStack.rend(); ++_it) {
        (*_it)->OnEvent(e);
        if (e.Handled) {
            break;
        }
    }
}

void Application::Run() {
    m_running = true;

    while (m_running) {
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        m_vertexArray->Bind();
        m_shader->Bind();
        glDrawElements(GL_TRIANGLES, m_vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        for (auto& _layer : m_layerStack) {
            _layer->OnUpdate();
        }
        // Render ImGui layer
        m_imguiLayer->Begin();
        for (Layer* _layer : m_layerStack) {
            _layer->OnImGuiRender();
        }
        m_imguiLayer->End();

        m_window->OnUpdate();
    }
}

bool Application::onWindowClose(WindowCloseEvent& event) {
    LOG_ENGINE_TRACE("Shutting down program...");
    m_running = false;
    return true;
}

void Application::PushLayer(Layer* layer) {
    m_layerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    m_layerStack.PushOverlay(overlay);
    overlay->OnAttach();
}
}  // namespace de
