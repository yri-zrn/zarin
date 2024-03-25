#pragma once

#include "Core/Core.hpp"
#include "Core/Layer.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/TextEvent.hpp"

#include <imgui.h>
#include <imgui_spectrum.h>
#include <ImGuizmo.h>

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace zrn {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& event) override;

    virtual void OnImGuiRender() override;

    void Begin();
    void End();

    void BlockEvents(bool block) { m_BlockEvents = block; }

private:
    bool m_BlockEvents = true;
    float m_Time = 0.0f;
};

} // namespace zrn
