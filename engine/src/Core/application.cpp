#include "Core/application.h"

#include <glad/glad.h>

#include "Core/log.h"
#include "Input/input.h"

namespace de {
Application* Application::instance = nullptr;

static GLenum ShaderDataTypeToOpenGLType(const ShaderDataType& type) {
    switch (type) {
        case ShaderDataType::None:
            return -1;
        case ShaderDataType::Vec:
        case ShaderDataType::Vec2:
        case ShaderDataType::Vec3:
        case ShaderDataType::Vec4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
    }
}

Application::Application() {
    if (instance) {
        LOG_ENGINE_CRITICAL("Application already exists! Aborting...");
        throw std::runtime_error("Application already exists! Aborting...");
    }

    instance = this;

    window = std::make_unique<Window>(WindowProps());
    window->SetEventCallback([this](Event& e) { OnEvent(e); });

    imguiLayer = new ImGuiLayer();
    PushOverlay(imguiLayer);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    float _vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.7f, 1.0f, 0.5f, -0.5f, 0.0f, 0.9f,
                              0.7f,  0.0f,  1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.3f,  1.0f};

    vertexBuffer.reset(VertexBuffer::Create(_vertices, sizeof(_vertices)));

    {
        BufferLayout _layout = {{ShaderDataType::Vec3, "position"}, {ShaderDataType::Vec4, "color"}};
        vertexBuffer->SetLayout(_layout);
    }

    auto _layout = vertexBuffer->GetLayout();
    unsigned _i = 0;
    for (const auto& element : _layout) {
        glEnableVertexAttribArray(_i);
        glVertexAttribPointer(_i, element.GetComponentCount(), ShaderDataTypeToOpenGLType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE, _layout.GetStride(),
                              (void*)(uintptr_t)element.offset);
        ++_i;
    }

    uint32_t _indices[3] = {0, 1, 2};
    indexBuffer.reset(IndexBuffer::Create(_indices, sizeof(_indices) / sizeof(uint32_t)));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

    std::string vertexSrc = R"(
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

    std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 o_position;
            in vec4 o_color;

            void main() {
                color = vec4(o_position * 0.5 + 0.5, 0.1);
                color = o_color;
            }
        )";

    shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
}

Application::~Application() = default;

void Application::OnEvent(Event& e) {
    EventDispatcher _eventDispatcher(e);
    _eventDispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return onWindowClose(event); });
    // Handle layer events
    for (auto _it = layerStack.rbegin(); _it != layerStack.rend(); ++_it) {
        (*_it)->OnEvent(e);
        if (e.Handled) {
            break;
        }
    }
}

void Application::Run() {
    running = true;

    while (running) {
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vertexArray);
        shader->Bind();
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

        for (auto& _layer : layerStack) {
            _layer->OnUpdate();
        }
        // Render ImGui layer
        imguiLayer->Begin();
        for (Layer* _layer : layerStack) {
            _layer->OnImGuiRender();
        }
        imguiLayer->End();

        window->OnUpdate();
    }
}

bool Application::onWindowClose(WindowCloseEvent& event) {
    LOG_ENGINE_DEBUG("Shutting down program...");
    running = false;
    return true;
}

void Application::PushLayer(Layer* layer) {
    layerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    layerStack.PushOverlay(overlay);
    overlay->OnAttach();
}
}  // namespace de
