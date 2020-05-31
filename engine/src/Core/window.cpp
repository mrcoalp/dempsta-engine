#include "Core/window.h"

#include <GLFW/glfw3.h>

#include "Core/core.h"
#include "Core/log.h"
#include "Events/applicationevent.h"
#include "Events/keyevent.h"
#include "Events/mouseevent.h"

namespace de {
static bool GLFWInitialized = false;

Window::Window(const WindowProps& windowProps) {
    data.title = windowProps.title;
    data.width = windowProps.width;
    data.height = windowProps.height;

    if (!GLFWInitialized) {
        int _statusGLFW = glfwInit();
        DE_ASSERT(_statusGLFW, "GLFW not initialized!")
        GLFWInitialized = true;
    }

    window =
        glfwCreateWindow((int)windowProps.width, (int)windowProps.height, windowProps.title.c_str(), nullptr, nullptr);

    // Initialize context
    context = new OpenGLContext(window);
    context->Init();

    glfwSetWindowUserPointer(window, &data);
    // Set, by default, vsync to true
    SetVSync(true);

    LOG_ENGINE_TRACE("Created window \"{0}\" ({1}x{2})...", data.title, data.width, data.height);

    // GLFW callbacks
    setGLFWCallbacks();
}

void Window::setGLFWCallbacks() {
    glfwSetErrorCallback(
        [](int error, const char* description) { LOG_ENGINE_ERROR("GLFW error ({0}): {2}", error, description); });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* pWwindow, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        data.width = width;
        data.height = height;
        data.eventQueue.push(new WindowResizeEvent(width, height));
    });

    glfwSetWindowIconifyCallback(window, [](GLFWwindow* pWwindow, int iconify) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        data.eventQueue.push(new WindowIconifyEvent(iconify));
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow* pWwindow) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        data.eventQueue.push(new WindowCloseEvent());
    });

    glfwSetCharCallback(window, [](GLFWwindow* pWwindow, unsigned int keyCode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        data.eventQueue.push(new KeyTypedEvent(keyCode));
    });

    glfwSetKeyCallback(window, [](GLFWwindow* pWwindow, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        switch (action) {
            case GLFW_PRESS: {
                data.eventQueue.push(new KeyPressedEvent(key, 0));
                break;
            }
            case GLFW_RELEASE: {
                data.eventQueue.push(new KeyReleasedEvent(key));
                break;
            }
            case GLFW_REPEAT: {
                data.eventQueue.push(new KeyPressedEvent(key, 1));
                break;
            }
            default: {
                LOG_ENGINE_ERROR("Invalid key action!");
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* pWwindow, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        switch (action) {
            case GLFW_PRESS: {
                data.eventQueue.push(new MouseBtnPressedEvent(button));
                break;
            }
            case GLFW_RELEASE: {
                data.eventQueue.push(new MouseBtnReleasedEvent(button));
                break;
            }
            default: {
                LOG_ENGINE_ERROR("Invalid mouse action!");
                break;
            }
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* pWwindow, double offsetX, double offsetY) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        data.eventQueue.push(new MouseScrolledEvent((float)offsetX, (float)offsetY));
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* pWwindow, double x, double y) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        data.eventQueue.push(new MouseMovedEvent((float)x, (float)y));
    });

    LOG_ENGINE_TRACE("GLFW callbacks initialized!");
}

Window::~Window() { shutdown(); }

void Window::shutdown() { glfwDestroyWindow(window); }

void Window::OnUpdate() {
    glfwPollEvents();

    while (!data.eventQueue.empty()) {
        data.EventCallback(*data.eventQueue.front());
        delete data.eventQueue.front();
        data.eventQueue.pop();
    }

    context->SwapBuffers();
}

void Window::SetVSync(bool enabled) {
    glfwSwapInterval((int)enabled);
    data.VSync = enabled;
}

bool Window::IsVSync() const { return data.VSync; }
}  // namespace de
