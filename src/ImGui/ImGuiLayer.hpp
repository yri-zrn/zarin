#pragma once

#include "Zarin/Core.hpp"
#include "Zarin/Layer.hpp"

#include "Zarin/Events/ApplicationEvent.hpp"
#include "Zarin/Events/KeyEvent.hpp"
#include "Zarin/Events/MouseEvent.hpp"
#include "Zarin/Events/TextEvent.hpp"

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
