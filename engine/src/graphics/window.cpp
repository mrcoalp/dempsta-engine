#include "Core/window.h"

namespace de {
    static bool GLFWInitialized = false;

    Window::Window(const WindowProps& windowProps) {
        data.title = windowProps.title;
        data.width = windowProps.width;
        data.height = windowProps.height;

        LOG_ENGINE_INFO("Creating window {0} ({1}x{2})", data.title, data.width, data.height);

        if (!GLFWInitialized) {
            int success = glfwInit();
            LOG_ENGINE_INFO("GLFWInit ({0})", success);
            GLFWInitialized = true;
        }

        window = glfwCreateWindow(
                (int) windowProps.width,
                (int) windowProps.height,
                windowProps.title.c_str(),
                nullptr,
                nullptr
        );

        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, &data);
        SetVSync(true);
    }

    Window::~Window() {
        Shutdown();
    }

    void Window::Shutdown() {
        glfwDestroyWindow(window);
    }

    void Window::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    void Window::SetVSync(bool enabled) {
        glfwSwapInterval((int) enabled);
        data.VSync = enabled;
    }

    bool Window::IsVSync() const {
        return data.VSync;
    }
}
