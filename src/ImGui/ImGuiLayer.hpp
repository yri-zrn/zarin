#pragma once

#include "Zarin/Core.hpp"
#include "Zarin/Layer.hpp"
#include "Zarin/Events/ApplicationEvent.hpp"
#include "Zarin/Events/KeyEvent.hpp"
#include "Zarin/Events/MouseEvent.hpp"
#include "Zarin/Events/TextEvent.hpp"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_vulkan.h"

#include "imgui.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

namespace zrn {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(Event& event) override;
    
private:
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseScrolledEvent(MouseScrolledEvent& e);
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    bool OnKeyReleasedEvent(KeyReleasedEvent& e);
    bool OnTextInputEvent(TextInputEvent& e);
    bool OnWindowResizeEvent(WindowResizeEvent& e);

private:
    SDL_Window* m_WindowHandle;
    ImGui_ImplVulkanH_Window* m_ImGuiWindowData;
};

} // namespace zrn
