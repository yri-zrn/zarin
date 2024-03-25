#pragma once

#include "Core/Core.hpp"
#include "Core/Timestep.hpp"
#include "Events/Event.hpp"

namespace zrn {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() { }
    virtual void OnDetach() { }
    virtual void OnUpdate(Timestep ts) { }
    virtual void OnImGuiRender() { }
    virtual void OnEvent(Event& event) { }

    inline const std::string& GetName() const { return m_DebugName; }
protected:
    std::string m_DebugName;
};

} // namespace zrn