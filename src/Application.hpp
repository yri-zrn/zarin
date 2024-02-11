#pragma once

#include "Core.hpp"

#include "Events/Event.hpp"
#include "Window.hpp"
#include "LayerStack.hpp"

namespace zrn {

class Application {
public:
    Application();
    virtual ~Application();
    
    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    LayerStack m_LayerStack;
};

Application* CreateApplication();

} // namespace zrn