#pragma once

#include "Core/application.h"

/**
 * Entry method to run a Dempsta app.
 * Create a new class that extends de::Application and define CreateApplication method to return
 * a new instance of that class:
 *
 * class Editor : public de::Application {
 * public:
 *      Editor() { PushLayer(new DempstaEditor()); };
 *      ~Editor() override = default;
 * };
 *
 * de::Application* de::CreateApplication() { return new Editor(); }
 */
int main(int argc, char** argv) {
    de::Log::Init();

    LOG_ENGINE_DEBUG(
        "\n************************************************************\n"
        "*                    WELCOME TO DEMPSTA                    *\n"
        "************************************************************");
    LOG_ENGINE_TRACE("Running initialization...");

    auto app = de::CreateApplication();
    app->Run();
    delete app;
}
