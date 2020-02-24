#include "Core/application.h"

namespace de {
    Application* Application::instance = nullptr;

    Application::Application() {
        if (instance) {
            LOG_ENGINE_CRITICAL("Application already exists! Aborting...");
            throw;
        }

        instance = this;

        window = std::make_unique<Window>(WindowProps());
        window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application() = default;

    void Application::OnEvent(Event& e) {
        EventDispatcher eventDispatcher(e);
        eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

        for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it) {
            (*it)->OnEvent(e);
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

            for (auto& layer : layerStack) {
                layer->OnUpdate();
            }

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
    }

    void Application::PushOverlay(Layer* overlay) {
        layerStack.PushOverlay(overlay);
    }
}
