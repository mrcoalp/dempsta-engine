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
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.width = width;
        _data.height = height;
        WindowResizeEvent _event(width, height);
        _data.EventCallback(_event);
    });

    glfwSetWindowIconifyCallback(window, [](GLFWwindow* pWwindow, int iconify) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        WindowIconifyEvent _event(iconify);
        _data.EventCallback(_event);
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow* pWwindow) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        WindowCloseEvent _event;
        _data.EventCallback(_event);
    });

    glfwSetCharCallback(window, [](GLFWwindow* pWwindow, unsigned int keyCode) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.eventQueue.push(new KeyTypedEvent((int)keyCode));
    });

    glfwSetKeyCallback(window, [](GLFWwindow* pWwindow, int key, int scancode, int action, int mods) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        switch (action) {
            case GLFW_PRESS: {
                _data.eventQueue.push(new KeyPressedEvent(key, 0));
                break;
            }
            case GLFW_RELEASE: {
                _data.eventQueue.push(new KeyReleasedEvent(key));
                break;
            }
            case GLFW_REPEAT: {
                _data.eventQueue.push(new KeyPressedEvent(key, 1));
                break;
            }
            default: {
                LOG_ENGINE_ERROR("Invalid key action!");
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* pWwindow, int button, int action, int mods) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        switch (action) {
            case GLFW_PRESS: {
                _data.eventQueue.push(new MouseBtnPressedEvent(button));
                break;
            }
            case GLFW_RELEASE: {
                _data.eventQueue.push(new MouseBtnReleasedEvent(button));
                break;
            }
            default: {
                LOG_ENGINE_ERROR("Invalid mouse action!");
                break;
            }
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* pWwindow, double offsetX, double offsetY) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.eventQueue.push(new MouseScrolledEvent((float)offsetX, (float)offsetY));
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* pWwindow, double x, double y) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.eventQueue.push(new MouseMovedEvent((float)x, (float)y));
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
