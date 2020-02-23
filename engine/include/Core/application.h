#pragma once

#include "log.h"

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
        static Application* instance;

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
