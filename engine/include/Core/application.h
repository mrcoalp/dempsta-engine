#pragma once

#include "Core/core.h"
#include "Core/layer.h"
#include "Core/layerstack.h"
#include "Core/window.h"
#include "Events/applicationevent.h"
#include "ImGui/imguilayer.h"

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
    inline static Application& GetInstance() { return *m_instance; }

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
    [[nodiscard]] inline Window& GetWindow() const { return *m_window; }

    void Close();

private:
    /**
     * @brief Application instance.
     * Application is a singleton.
     */
    static Application* m_instance;
    /**
     * @brief Window object pointer.
     */
    Scope<Window> m_window;
    /**
     * @brief Whether or not the application is running.
     */
    bool m_running = false;
    LayerStack m_layerStack;
    float m_lastFrameTime = 0.0f;
    bool m_minimized = false;

#ifdef NDEBUG
    ImGuiLayer* m_imguiLayer;
#endif

    /**
     * @brief On window close event callback.
     * @param event
     * @return
     */
    bool onWindowClose(WindowCloseEvent& event);

    /**
     * On window resize event callback.
     * @param event
     * @return
     */
    bool onWindowResize(WindowResizeEvent& event);

    /**
     * On window iconify/minimized event callback.
     * @param event
     * @return
     */
    bool onWindowIconify(WindowIconifyEvent& event);

    /**
     * @brief Main function entry.
     * @param argc
     * @param argv
     * @return
     */
    friend int ::main(int argc, char** argv);
};

/**
 * @brief Creates a new DEMPSTA ENGINE Application.
 * Used as an entry point to be defined by client app.
 * @return A new de::Application.
 */
Application* CreateApplication();
}  // namespace de
