#pragma once

#include "Zarin/Layer.hpp"

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
    SDL_Window* m_WindowHandle;
    ImGui_ImplVulkanH_Window* m_ImGuiWindowData;
};

} // namespace zrn
