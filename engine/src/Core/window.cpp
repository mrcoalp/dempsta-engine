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
    m_data.title = windowProps.title;
    m_data.width = windowProps.width;
    m_data.height = windowProps.height;

    if (!GLFWInitialized) {
        int _statusGLFW = glfwInit();
        DE_ASSERT(_statusGLFW != 0, "GLFW not initialized!")
        GLFWInitialized = true;
    }

    m_window = glfwCreateWindow((int)windowProps.width, (int)windowProps.height, windowProps.title.c_str(), nullptr, nullptr);

    // Initialize context
    m_context = new OpenGLContext(m_window);
    m_context->Init();

    glfwSetWindowUserPointer(m_window, &m_data);
    // Set, by default, vsync to true
    SetVSync(true);

    LOG_ENGINE_TRACE("Created window \"{0}\" ({1}x{2})...", m_data.title, m_data.width, m_data.height);

    // GLFW callbacks
    setGLFWCallbacks();
}

void Window::setGLFWCallbacks() {
    glfwSetErrorCallback([](int error, const char* description) { LOG_ENGINE_ERROR("GLFW error ({0}): {2}", error, description); });

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* pWwindow, int width, int height) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.width = width;
        _data.height = height;
        WindowResizeEvent _event(width, height);
        _data.EventCallback(_event);
    });

    glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* pWwindow, int iconify) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        WindowIconifyEvent _event(iconify != 0);
        _data.EventCallback(_event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* pWwindow) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        WindowCloseEvent _event;
        _data.EventCallback(_event);
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* pWwindow, unsigned int keyCode) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.eventQueue.push(new KeyTypedEvent((int)keyCode));
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* pWwindow, int key, int scancode, int action, int mods) {
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

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* pWwindow, int button, int action, int mods) {
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

    glfwSetScrollCallback(m_window, [](GLFWwindow* pWwindow, double offsetX, double offsetY) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.eventQueue.push(new MouseScrolledEvent((float)offsetX, (float)offsetY));
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* pWwindow, double x, double y) {
        WindowData& _data = *(WindowData*)glfwGetWindowUserPointer(pWwindow);
        _data.eventQueue.push(new MouseMovedEvent((float)x, (float)y));
    });

    LOG_ENGINE_TRACE("GLFW callbacks initialized!");
}

Window::~Window() { shutdown(); }

void Window::shutdown() { glfwDestroyWindow(m_window); }

void Window::OnUpdate() {
    glfwPollEvents();

    while (!m_data.eventQueue.empty()) {
        m_data.EventCallback(*m_data.eventQueue.front());
        delete m_data.eventQueue.front();
        m_data.eventQueue.pop();
    }

    m_context->SwapBuffers();
}

void Window::SetVSync(bool enabled) {
    glfwSwapInterval((int)enabled);
    m_data.VSync = enabled;
}

bool Window::IsVSync() const { return m_data.VSync; }
}  // namespace de
