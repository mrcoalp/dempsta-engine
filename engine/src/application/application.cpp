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
    }

    Application::~Application() {
        delete instance;
    }

    void Application::Run() {
        running = true;

        while (running) {
            glClearColor(0, 1, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            window->OnUpdate();
        }
    }
}
