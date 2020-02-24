#include "Core/application.h"

namespace de {
#define BIND_EVENT_FN(fn)    std::bind(&fn, this, std::placeholders::_1)

    Application* Application::instance = nullptr;

    Application::Application() {
        if (instance) {
            LOG_ENGINE_CRITICAL("Application already exists! Aborting...");
            throw;
        }

        instance = this;

        window = std::make_unique<Window>(WindowProps());
        window->SetEventCallback(BIND_EVENT_FN(Application::onEvent));
    }

    Application::~Application() = default;

    void Application::onEvent(Event& e) {
        EventDispatcher eventDispatcher(e);
        eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
    }

    void Application::run() {
        running = true;

        while (running) {
            glClearColor(0, 1, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            window->OnUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& event) {
        LOG_ENGINE_DEBUG("Closed window! Shutting down program...");
        running = false;
        return true;
    }
}
