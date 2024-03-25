#include "Window.hpp"

#include "Platform/Windows/WindowsWindow.hpp"

namespace zrn {

Scope<Window> Window::Create(const WindowProps& props) {
    return CreateScope<WindowsWindow>(props);
}

} // namespace zrn