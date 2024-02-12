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

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *s_Instance; }

private:
    
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    LayerStack m_LayerStack;

private:
    static Application* s_Instance;
};

Application* CreateApplication();

} // namespace zrn