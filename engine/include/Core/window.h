#pragma once

#include <queue>

#include "Events/event.h"
#include "Platform/OpenGL/openglcontext.h"

namespace de {
/**
 * @brief Contains relevant window properties.
 */
struct WindowProps {
    std::string title = "Default Dempsta Title";
    unsigned int width = 1280u;
    unsigned int height = 720u;
};

class Window {
public:
    /**
     * @brief Event callback function is a void function that receives an event.
     */
    using EventCallbackFn = std::function<void(Event&)>;

    explicit Window(const WindowProps& windowProps);

    ~Window();

    /**
     * @brief Called once per frame. Checks for window updates.
     */
    void OnUpdate();

    /**
     * @brief Getter for the window width.
     * @return Window width.
     */
    [[nodiscard]] inline unsigned int GetWidth() const { return m_data.width; }

    /**
     * @brief Getter for the window height.
     * @return Window height.
     */
    [[nodiscard]] inline unsigned int GetHeight() const { return m_data.height; }

    /**
     * @brief Event callback function setter in WindowData.
     * @param callback Function to define callback.
     */
    inline void SetEventCallback(const EventCallbackFn& callback) { m_data.EventCallback = callback; }

    /**
     * @brief VSync attribute setter
     * @param enabled Whether or not to enable vsync.
     */
    void SetVSync(bool enabled);

    /**
     * @brief Getter for vsync.
     * @return
     */
    [[nodiscard]] bool IsVSync() const;

    /**
     * @brief Getter for glfw window pointer.
     * @return
     */
    [[nodiscard]] inline GLFWwindow* GetNativeWindow() const { return m_window; }

private:
    /**
     * @brief Destroys current window.
     */
    void shutdown();

    /**
     * @brief Initializes glfw callback functions.
     */
    void setGLFWCallbacks();

    GLFWwindow* m_window;

    struct WindowData {
        std::string title;
        unsigned int width;
        unsigned int height;
        bool VSync;
        EventCallbackFn EventCallback;
        std::queue<Event*> eventQueue;
    };

    WindowData m_data;

    OpenGLContext* m_context;
};
}  // namespace de
