#include "Core/application.h"

#include "Core/input.h"
#include "Core/log.h"
#include "Core/core.h"

namespace de {
    Application* Application::instance = nullptr;

    Application::Application() {
        if (instance) {
            LOG_ENGINE_CRITICAL("Application already exists! Aborting...");
            throw;
        }

        instance = this;

        window = std::make_unique<Window>(WindowProps());
        window->SetEventCallback(DE_BIND_EVENT_FN(Application::OnEvent));

        imguiLayer = new ImGuiLayer();
        PushOverlay(imguiLayer);
    }

    Application::~Application() = default;

    void Application::OnEvent(Event& e) {
        EventDispatcher _eventDispatcher(e);
        _eventDispatcher.Dispatch<WindowCloseEvent>(DE_BIND_EVENT_FN(Application::onWindowClose));

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
        LOG_ENGINE_DEBUG("{0}! Shutting down program...", event.ToString());
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
