#include "Application.hpp"

#include "Log.hpp"
#include "Input/Input.hpp"

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

        // IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
        // IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button);}

        // auto[x, y] = Input::GetMousePosition();
        // ZRN_TRACE("Mouse position: {0}, {1}", x, y);

        // if (Input::IsMouseButtonPressed(Button::Left)) {
        //     ZRN_TRACE("Left mouse button pressed");
        // }
        // if (Input::IsMouseButtonPressed(Button::Middle)) {
        //     ZRN_TRACE("Middle mouse button pressed");
        // }
        // if (Input::IsMouseButtonPressed(Button::Right)) {
        //     ZRN_TRACE("Right mouse button pressed");
        // }
        // if (Input::IsKeyPressed(Key::a)) {
        //     ZRN_TRACE("Key \'a\' pressed");
        // }
        // if (Input::IsKeyPressed(Key::D_1)) {
        //     ZRN_TRACE("Key \'1\' pressed");
        // }
        // if (Input::IsKeyPressed(Key::Space)) {
        //     ZRN_TRACE("Key \'Space\' pressed");
        // }
        // if (Input::IsKeyPressed(Key::KP_Period)) {
        //     ZRN_TRACE("Key \'KP_Period\' pressed");
        // }
        // if (Input::IsKeyPressed(Key::Minus)) {
        //     ZRN_TRACE("Key \'Minus\' pressed");
        // }
        
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