#pragma once

#include "Core/log.h"
#include "Core/window.h"
#include "Core/core.h"
#include "Core/layerstack.h"

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

        /**
         * @brief Initializes application.
         */
        void Run();

        /**
         * @brief Handle on event.
         * @param e Event to be handled.
         */
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);

        void PushOverlay(Layer* overlay);

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
        LayerStack layerStack;

        /**
         * @brief On window close event callback.
         * @param event
         * @return
         */
        bool onWindowClose(WindowCloseEvent& event);

        /**
         * @brief Main function entry.
         * @param argc
         * @param argv
         * @return
         */
        friend int::main(int argc, char** argv);
    };

    /**
      * @brief Creates a new DEMPSTA ENGINE Application.
      * Used as an entry point to be defined by client app.
      * @return A new de::Application.
      */
    Application* CreateApplication();
}  // namespace de
