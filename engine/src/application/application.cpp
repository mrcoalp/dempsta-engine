#include "application.h"

namespace de {
    Application* Application::instance = nullptr;

    Application::Application() {
        if (instance) {
            LOG_ENGINE_ERROR("Application already exists! Aborting...");
            throw;
        }

        instance = this;
    }

    Application::~Application() {
        delete instance;
    }

    void Application::Run() {
        while (true);
    }
}
