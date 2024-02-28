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

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *s_Instance; }

private:
    bool OnWindowClose(WindowCloseEvent& e);

private:
    ImGuiLayer* m_ImGuiLayer;
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    LayerStack m_LayerStack;
    
    float m_LastTime = 0.0f;

    uint64_t last;

private:
    static Application* s_Instance;
};

Application* CreateApplication();

} // namespace zrn