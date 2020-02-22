#include "application.h"

using namespace de;

Application::Application() {}

Application::~Application() {}

void Application::Run() {
    Log::Init();
    LOG_ENGINE_INFO("Running initialization...");
}
