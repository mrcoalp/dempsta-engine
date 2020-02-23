#pragma once

#include "Core/log.h"
#include "Core/window.h"

int main(int argc, char** argv);

namespace de {
    class Application {
    public:
        Application();

        virtual ~Application();

        /**
         * @brief Getter for the app instance.
         * @return Application instance.
         */
        inline static Application& GetInstance() { return *instance; }

    private:
        /**
         * @brief Application instance.
         * Application is a singleton.
         */
        static Application* instance;
        /**
         * @brief Window object pointer.
         */
        std::unique_ptr<Window> window;
        /**
         * @brief Whether or not the application is running.
         */
        bool running = false;

        /**
         * @brief Initializes application.
         */
        void Run();

        friend int::main(int argc, char** argv);
    };
}  // namespace de

/**
  * @brief Creates a new DEMPSTA ENGINE Application.
  * @return Used as an entry point to be defined by client app.
  */
de::Application* CreateApplication();
