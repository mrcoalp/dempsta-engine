#pragma once

#include "Core/application.h"

int main(int argc, char** argv) {
    de::Log::Init();
    LOG_ENGINE_INFO("Running initialization...");
    auto app = CreateApplication();
    app->Run();
    delete app;
}
