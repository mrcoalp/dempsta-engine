#pragma once

#include "Core/application.h"

int main(int argc, char** argv) {
    de::Log::Init();

    LOG_ENGINE_DEBUG("\n************************************************************\n"
                     "*                    WELCOME TO DEMPSTA                    *\n"
                     "************************************************************");
    LOG_ENGINE_TRACE("Running initialization...");

    auto app = CreateApplication();
    app->Run();
    delete app;
}
