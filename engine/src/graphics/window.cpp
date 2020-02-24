#include "Core/window.h"

namespace de {
    static bool GLFWInitialized = false;

    static void GLFWOnError(int error, const char* description) {
        LOG_ENGINE_ERROR("GLFW error ({0}): {2}", error, description);
    }

    Window::Window(const WindowProps& windowProps) {
        data.title = windowProps.title;
        data.width = windowProps.width;
        data.height = windowProps.height;

        if (!GLFWInitialized) {
            int success = glfwInit();

            if (success == 1) {
                LOG_ENGINE_INFO("GLFW initialized successfully!");
                GLFWInitialized = true;
            } else {
                LOG_ENGINE_CRITICAL("GLFW initialization failed!");
                throw;
            }
        }

        LOG_ENGINE_TRACE("Creating window \"{0}\" ({1}x{2})...", data.title, data.width, data.height);

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

        LOG_ENGINE_INFO("Window created!");

        // GLFW callbacks
        setGLFWCallbacks();
    }

    void Window::setGLFWCallbacks() {
        LOG_ENGINE_TRACE("Initializing glfw callbacks...");

        glfwSetErrorCallback(GLFWOnError);

        glfwSetWindowSizeCallback(window, [](GLFWwindow* pWwindow, int width, int height) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(pWwindow);
            data.width = width;
            data.height = height;
            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* pWwindow) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(pWwindow);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* pWwindow, int key, int scancode, int action, int mods) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(pWwindow);
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
                default: {
                    LOG_ENGINE_ERROR("Invalid key action!");
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* pWwindow, int button, int action, int mods) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(pWwindow);
            switch (action) {
                case GLFW_PRESS: {
                    MouseBtnPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseBtnReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                default: {
                    LOG_ENGINE_ERROR("Invalid mouse action!");
                    break;
                }
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* pWwindow, double offsetX, double offsetY) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(pWwindow);
            MouseScrolledEvent event((float) offsetX, (float) offsetY);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* pWwindow, double x, double y) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(pWwindow);
            MouseMovedEvent event((float) x, (float) y);
            data.EventCallback(event);
        });

        LOG_ENGINE_INFO("Callbacks initialized!");
    }

    Window::~Window() {
        shutdown();
    }

    void Window::shutdown() {
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
