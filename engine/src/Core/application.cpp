#include "Core/application.h"

#include <GLFW/glfw3.h>

#include "Renderer/renderer.h"
#include "Scripting/API/databuffer.h"
#include "Scripting/API/luaentity.h"
#include "Scripting/scriptengine.h"

namespace de {
static void RegisterScriptingAPI() {
    lua::DataBuffer::Register(SE::GetState());
    SE::RegisterClass<lua::LuaEntity>();
}

Application* Application::m_instance = nullptr;

Application::Application() : Application(WindowProps()) {}

Application::Application(const WindowProps& windowProps) {
    DE_ASSERT(m_instance == nullptr, "Application already exists! Aborting...")

    m_instance = this;

    m_window = CreateScope<Window>(windowProps);
    m_window->SetEventCallback([this](Event& e) { OnEvent(e); });

    Renderer::Init();
    SE::Init();
    RegisterScriptingAPI();

    m_imguiLayer = new ImGuiLayer();
    PushOverlay(m_imguiLayer);
}

Application::~Application() { SE::CloseState(); }

void Application::OnEvent(Event& e) {
    EventDispatcher eventDispatcher(e);
    eventDispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return onWindowClose(event); });
    eventDispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent& event) { return onWindowResize(event); });
    eventDispatcher.Dispatch<WindowIconifyEvent>([this](WindowIconifyEvent& event) { return onWindowIconify(event); });

    // Handle layer events
    for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
        (*it)->OnEvent(e);
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

        // Render ImGui layer
        ImGuiLayer::Begin();
        for (const auto& _layer : m_layerStack) {
            _layer->OnImGuiRender();
        }
        ImGuiLayer::End();

        m_window->OnUpdate();
    }
}

void Application::Close() { m_running = false; }

bool Application::onWindowClose(WindowCloseEvent& event) {
    Close();
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
