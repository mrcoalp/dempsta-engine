#include "Core/application.h"

#include <glad/glad.h>
#include "Input/input.h"
#include "Core/log.h"

namespace de {
    Application* Application::instance = nullptr;

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

        float _vertices[3 * 3] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
        };
        vertexBuffer.reset(VertexBuffer::Create(_vertices, sizeof(_vertices)));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        uint32_t _indices[3] = {0, 1, 2};
        indexBuffer.reset(IndexBuffer::Create(_indices, sizeof(_indices) / sizeof(uint32_t)));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 position;

            out vec3 o_position;

            void main() {
                o_position = position;
                gl_Position = vec4(position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 o_position;

            void main() {
                color = vec4(o_position * 0.5 + 0.5, 0.1);
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
            glClearColor(1, 1, 0, 1);
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
}
