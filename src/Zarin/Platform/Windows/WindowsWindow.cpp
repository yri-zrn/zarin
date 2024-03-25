#include "WindowsWindow.hpp"
#include "Zarin/Input/KeyCodes.hpp"
#include "Zarin/Input/MouseCodes.hpp"

#include "Zarin/Platform/OpenGL/OpenGLContext.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace zrn {

static uint8_t s_SDL2_WindowCount = 0;

static bool s_SDL2_Initialized = false;

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
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

    auto window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_WindowHandle = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.Width, props.Height, window_flags);

    // SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
    // SDL_SetRelativeMouseMode(SDL_TRUE);

    ZRN_CORE_ASSERT(m_WindowHandle != nullptr, "Failed to create SDL_Window");

    SDL_SetWindowData(m_WindowHandle, "Data", &m_Data);

    m_Context = CreateScope<OpenGLContext>(m_WindowHandle);
    m_Context->Init();

    // Register Event callbacks
    SDL_AddEventWatch(WindowCloseEventWatcher,  m_WindowHandle);
    SDL_AddEventWatch(WindowResizeEventWatcher, m_WindowHandle);
    SDL_AddEventWatch(KeyWatcher,               m_WindowHandle);
    SDL_AddEventWatch(MouseButtonWatcher,       m_WindowHandle);
    SDL_AddEventWatch(MouseWheelWatcher,        m_WindowHandle);
    SDL_AddEventWatch(MouseMotionWatcher,       m_WindowHandle);
    SDL_AddEventWatch(TextInputWatcher,         m_WindowHandle);
}

void WindowsWindow::Shutdown() {
    SDL_DestroyWindow(m_WindowHandle);

    SDL_Quit();
}

void WindowsWindow::OnUpdate() {
    m_Context->SwapBuffers();
    
    // SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}

int WindowsWindow::WindowCloseEventWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_WINDOWEVENT
    &&  event->window.event == SDL_WINDOWEVENT_CLOSE
    &&  window == (SDL_Window*)data) {
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
    if (event->type == SDL_KEYDOWN
    &&  window == (SDL_Window*)data) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        KeyCode keycode = (KeyCode)event->key.keysym.sym;
        KeyMod mod = (KeyMod)event->key.keysym.mod;

        int repeat_count = (int)event->key.repeat;

        KeyPressedEvent e{ keycode, mod, repeat_count };
        data.EventCallback(e);
    }
    else if (event->type == SDL_KEYUP
    &&  window == (SDL_Window*)data) {
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
    if (event->type == SDL_TEXTINPUT
    &&  window == (SDL_Window*)data) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        std::string text = event->text.text;

        TextInputEvent e{ text };
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::MouseButtonWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_MOUSEBUTTONDOWN
    &&  window == (SDL_Window*)data) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        MouseButton button = (MouseButton)event->button.button;

        MouseButtonPressedEvent e{ button };
        data.EventCallback(e);
    }
    else if (event->type == SDL_MOUSEBUTTONUP
    &&  window == (SDL_Window*)data) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        MouseButton button = (MouseButton)event->button.button;

        MouseButtonReleasedEvent e{ button };
        data.EventCallback(e);
    }
    return 0;
}

int WindowsWindow::MouseWheelWatcher(void* data, SDL_Event* event) {
    SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
    if (event->type == SDL_MOUSEWHEEL
    &&  window == (SDL_Window*)data) {
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
    if (event->type == SDL_MOUSEMOTION
    &&  window == (SDL_Window*)data) {
        WindowData& data = *(WindowData*)SDL_GetWindowData(window, "Data");
        float x = event->motion.x;
        float y = event->motion.y;

        MouseMovedEvent e{ x, y };
        data.EventCallback(e);
    }
    return 0;
}

} // namespace zrn