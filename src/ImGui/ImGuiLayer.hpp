#pragma once

#include "Zarin/Core.hpp"
#include "Zarin/Layer.hpp"

namespace zrn {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();

private:
    float m_Time = 0.0f; 
};

} // namespace zrn
