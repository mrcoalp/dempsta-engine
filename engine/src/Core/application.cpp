#include "Core/application.h"

#include <GLFW/glfw3.h>

#include "Core/core.h"
#include "Core/log.h"
#include "Input/input.h"
#include "Renderer/renderer.h"

namespace de {
Application* Application::m_instance = nullptr;

Application::Application() {
    DE_ASSERT(m_instance == nullptr, "Application already exists! Aborting...")

    m_instance = this;

    m_window = std::make_unique<Window>(WindowProps());
    m_window->SetEventCallback([this](Event& e) { OnEvent(e); });

    Renderer::Init();

#ifdef NDEBUG
    m_imguiLayer = new ImGuiLayer();
    PushOverlay(m_imguiLayer);
#endif
}

Application::~Application() = default;

void Application::OnEvent(Event& e) {
    EventDispatcher _eventDispatcher(e);
    _eventDispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return onWindowClose(event); });
    _eventDispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) { return onWindowResize(event); });
    _eventDispatcher.Dispatch<WindowIconifyEvent>([this](WindowIconifyEvent& event) { return onWindowIconify(event); });

    // Handle layer events
    for (auto _it = m_layerStack.rbegin(); _it != m_layerStack.rend(); ++_it) {
        (*_it)->OnEvent(e);
        if (e.Handled) {
            break;
        }
    }
}

void Application::Run() {
    m_running = true;

    while (m_running) {
        auto _time = (float)glfwGetTime();
        TimeStep _ts(_time - m_lastFrameTime);
        m_lastFrameTime = _time;

        if (!m_minimized) {
            for (const auto& _layer : m_layerStack) {
                _layer->OnUpdate(_ts);
            }
        }
#ifdef NDEBUG
        // Render ImGui layer
        m_imguiLayer->Begin();
        for (const auto& _layer : m_layerStack) {
            _layer->OnImGuiRender();
        }
        m_imguiLayer->End();
#endif

        m_window->OnUpdate();
    }
}

bool Application::onWindowClose(WindowCloseEvent& event) {
    m_running = false;
    return true;
}

bool Application::onWindowResize(WindowResizeEvent& event) {
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    return false;
}

bool Application::onWindowIconify(WindowIconifyEvent& event) {
    m_minimized = event.GetIconify();
    return false;
}

void Application::PushLayer(Layer* layer) {
    m_layerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    m_layerStack.PushOverlay(overlay);
    overlay->OnAttach();
}
}  // namespace de
