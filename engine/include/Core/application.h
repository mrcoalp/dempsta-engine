#pragma once

#include "Core/window.h"
#include "Core/layer.h"
#include "Core/layerstack.h"
#include "ImGui/imguilayer.h"
#include "Events/applicationevent.h"
#include "Renderer/shader.h"

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

        /**
         * @brief Pushes a new layer to layerstack and attaches it.
         * @param layer Layer to be pushed
         */
        void PushLayer(Layer* layer);

        /**
         * @brief Pushes a new layer as an overlay to layerstack and attaches it.
         * @param overlay Layer to be pushed as an overlay.
         */
        void PushOverlay(Layer* overlay);

        /**
         * @brief Getter for the window.
         * @return A pointer to the window
         */
        [[nodiscard]] inline Window& GetWindow() const { return *window; }

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
        ImGuiLayer* imguiLayer;
        unsigned int vertexArray, bufferArray, indexBuffer;
        std::unique_ptr<Shader> shader;

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
