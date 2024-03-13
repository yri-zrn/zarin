#pragma once

#include "Zarin/Window.hpp"
#include "Zarin/Input/Input.hpp"
#include "Zarin/Renderer/GraphicsContext.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace zrn {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline uint32_t GetWidth() const override { return m_Data.Width; }
    inline uint32_t GetHeight() const override { return m_Data.Height; }

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    
    // TODO: implement
    void SetVSync(bool enabled) { }
    bool IsVSync() const { return true; }

    inline virtual void* GetWindowHandle() const override { return (void*)m_WindowHandle; }

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

private:
    // Event callbacks
    static int WindowCloseEventWatcher(void* data, SDL_Event* event);
    static int WindowResizeEventWatcher(void* data, SDL_Event* event);
    static int KeyWatcher(void* data, SDL_Event* event);
    static int TextInputWatcher(void* data, SDL_Event* event);
    static int MouseButtonWatcher(void* data, SDL_Event* event);
    static int MouseWheelWatcher(void* data, SDL_Event* event);
    static int MouseMotionWatcher(void* data, SDL_Event* event);

private:
    SDL_Window* m_WindowHandle;
    Scope<GraphicsContext> m_Context;

    struct WindowData {
        std::string Title;
        uint32_t Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    } m_Data;
};

} // namespace zrn