#include "Application.hpp"

#include "Input/Input.hpp"
#include "Renderer/Renderer.hpp"

#include <iostream>

#include <SDL2/SDL.h>

namespace zrn {

#define DrawQuad
#define DrawTris

Application* Application::s_Instance = nullptr;

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application() {
    ZRN_CORE_ASSERT(!s_Instance, "Application already exists");
    s_Instance = this;

    m_Window = Window::Create();
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    Renderer::Init();

    last = SDL_GetPerformanceCounter();
}

Application::~Application() {
    Renderer::Shutdown();
}

void Application::Run() {
    while (m_Running) {
        uint64_t now = SDL_GetPerformanceCounter();
        Timestep timestep = (float)(now - last) / SDL_GetPerformanceFrequency();
        last = now;

        if (!m_Minimized) {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);
            
            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();
        }

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher{ e };
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        if (e.Handled)
            break;
        (*it)->OnEvent(e);
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

    return false;
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Application::Close() {
    m_Running = false;
}

} // namespace zrn