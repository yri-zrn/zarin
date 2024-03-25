#pragma once

#include "Core/Core.hpp"
#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/TextEvent.hpp"

namespace zrn {

struct WindowProps {
    std::string Title;
    uint32_t Width, Height;

    WindowProps(const std::string& title = "Zarin Engine",
                uint32_t width = 1280,
                uint32_t height = 720)
    : Title(title), Width(width), Height(height) { }
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() { }

    virtual void OnUpdate() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void* GetWindowHandle() const = 0;

    static Scope<Window> Create(const WindowProps& props = WindowProps());
};

} // namespace zrn


