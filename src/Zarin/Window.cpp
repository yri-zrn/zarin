#include "Window.hpp"

#include "Zarin/Platform/Windows/WindowsWindow.hpp"

namespace zrn {

Scope<Window> Window::Create(const WindowProps& props) {
    return CreateScope<WindowsWindow>(props);
}

} // namespace zrn