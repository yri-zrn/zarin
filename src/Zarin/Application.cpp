#include "Application.hpp"

#include "Input/Input.hpp"
#include "Zarin/Renderer/Renderer.hpp"

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

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    Renderer::Init();

    last = SDL_GetPerformanceCounter();
}

Application::~Application() {

}

void Application::Run() {
    while (m_Running) {
        uint64_t now = SDL_GetPerformanceCounter();
        Timestep timestep = (float)(now - last) / SDL_GetPerformanceFrequency();
        last = now;

        RenderCommand::SetClearColor({ 0.4f, 0.5f, 0.9, 1.0f });
        RenderCommand::Clear();

        for (Layer* layer : m_LayerStack)
            layer->OnUpdate(timestep);
        
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

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