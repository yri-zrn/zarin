#include "WindowsWindow.hpp"
#include "Zarin/Input/KeyCodes.hpp"
#include "Zarin/Input/MouseCodes.hpp"

namespace zrn {

static bool s_SDL2_Initialized = false;

Window* Window::Create(const WindowProps& props) {
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}

WindowsWindow::~WindowsWindow() {
    // Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    ZRN_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_SDL2_Initialized) {
        ZRN_CORE_ASSERT(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0, "Could not initialize SDL2");
        s_SDL2_Initialized = true;
    }

    auto window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_Window = SDL_CreateWindow(
        props.Title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        props.Width, props.Height,
        window_flags);

    ZRN_CORE_ASSERT(m_Window != nullptr, "Failed to create SDL_Window");
    
    auto renderer_flags = static_cast<SDL_RendererFlags>(SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    m_Renderer = SDL_CreateRenderer(
        m_Window,
        -1,
        renderer_flags);

    ZRN_CORE_ASSERT(m_Renderer != nullptr, "Failed to create SDL_Renderer");

    SDL_SetWindowData(m_Window, "Data", &m_Data);

    // Register Event callbacks
    SDL_AddEventWatch(WindowCloseEventWatcher,  m_Window);
    SDL_AddEventWatch(WindowResizeEventWatcher, m_Window);
    SDL_AddEventWatch(KeyWatcher,               m_Window);
    SDL_AddEventWatch(MouseButtonWatcher,       m_Window);
    SDL_AddEventWatch(MouseWheelWatcher,        m_Window);
    SDL_AddEventWatch(MouseMotionWatcher,       m_Window);
    SDL_AddEventWatch(TextInputWatcher,         m_Window);
}

void WindowsWindow::Shutdown() {
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);

    // TODO: multiple window support
    // SDL_Quit();
}

void WindowsWindow::OnUpdate() {    
    SDL_Event event;
    while(SDL_PollEvent(&event) > 0) { }

    SDL_SetRenderDrawColor(m_Renderer, 189, 215, 244, 255);
    SDL_RenderClear(m_Renderer);

    SDL_RenderPresent(m_Renderer);
}



void WindowsWindow::SetVSync(bool enabled) {

}

bool WindowsWindow::IsVSync() const {
    return m_Data.VSync;
}

int WindowsWindow::WindowCloseEventWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_WINDOWEVENT
    &&  event->window.event == SDL_WINDOWEVENT_CLOSE
    &&  window == (SDL_Window*)data) {
        // Should skip the event to remove it from stack?
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        WindowCloseEvent e;
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::WindowResizeEventWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_WINDOWEVENT
    &&  event->window.event == SDL_WINDOWEVENT_RESIZED
    &&  window == (SDL_Window*)data) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        
        data.Width  = event->window.data1;
        data.Height = event->window.data2;
        WindowResizeEvent e{ data.Width, data.Height };
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::KeyWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_KEYDOWN) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        KeyCode keycode = (KeyCode)event->key.keysym.sym;
        KeyMod mod = (KeyMod)event->key.keysym.mod;

        int repeat_count = (int)event->key.repeat;

        KeyPressedEvent e{ keycode, mod, repeat_count };
        data.EventCallback(e);
    }
    else if (event->type == SDL_KEYUP) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        KeyCode keycode = (KeyCode)event->key.keysym.sym;
        KeyMod mod = (KeyMod)event->key.keysym.mod;

        KeyReleasedEvent e{ keycode, mod };
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::TextInputWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_TEXTINPUT) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        std::string text = event->text.text;

        TextInputEvent e{ text };
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::MouseButtonWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        MouseButton button = (MouseButton)event->button.button;

        MouseButtonPressedEvent e{ button };
        data.EventCallback(e);
    }
    else if (event->type == SDL_MOUSEBUTTONUP) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        MouseButton button = (MouseButton)event->button.button;

        MouseButtonReleasedEvent e{ button };
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::MouseWheelWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_MOUSEWHEEL) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        float xOffset = event->wheel.preciseX;
        float yOffset = event->wheel.preciseY;

        MouseScrolledEvent e{ xOffset, yOffset };
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::MouseMotionWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_MOUSEMOTION) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        float x = event->motion.x;
        float y = event->motion.y;

        MouseMovedEvent e{ x, y };
        data.EventCallback(e);
    }
    return 0;
}

} // namespace zrn