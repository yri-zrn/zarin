#include "Application.hpp"
#include "Core.hpp"
#include "Log.hpp"

#include <iostream>

namespace zrn {

Application* Application::s_Instance = nullptr;

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application() {
    ZRN_CORE_ASSERT(!s_Instance, "Application already exists");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());

    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Application::~Application() {

}

void Application::Run() {
    while (m_Running) {
        for (Layer* layer : m_LayerStack)
            layer->OnUpdate();

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher{ e };
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
        (*--it)->OnEvent(e);
        if (e.Handled)
            break;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

} // namespace zrn