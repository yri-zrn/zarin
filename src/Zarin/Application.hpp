#pragma once

#include "Core.hpp"

#include "Events/Event.hpp"
#include "Window.hpp"
#include "LayerStack.hpp"

#include "ImGui/ImGuiLayer.hpp"

#include "Zarin/Core/Timestep.hpp"

#include "Zarin/Renderer/Shader.hpp"
#include "Zarin/Renderer/Buffer.hpp"
#include "Zarin/Renderer/VertexArray.hpp"
#include "Zarin/Renderer/Camera.hpp"

namespace zrn {

class Application {
public:

    Application();
    virtual ~Application();
    
    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    void Close();

    Window& GetWindow() { return *m_Window; }
    static Application& Get() { return *s_Instance; }

    ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

private:
    bool m_Running = true;
    bool m_Minimized = false;

    ImGuiLayer* m_ImGuiLayer;
    Scope<Window> m_Window;
    
    LayerStack m_LayerStack;
    
    float m_LastTime = 0.0f;

    uint64_t last;

private:
    static Application* s_Instance;
};

Application* CreateApplication();

} // namespace zrn