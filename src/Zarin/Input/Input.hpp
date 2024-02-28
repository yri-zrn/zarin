#pragma once

#include "Zarin/Core.hpp"

#include "Zarin/Input/KeyCodes.hpp"
#include "Zarin/Input/MouseCodes.hpp"

#include <glm/vec2.hpp>

namespace zrn {

class Input {
public:
    inline static bool IsKeyPressed(KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

    inline static bool IsMouseButtonPressed(MouseButton button) { return s_Instance->IsMouseButtonPressedImpl(button);}
    inline static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
    inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
    inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

protected:
    virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;

    virtual bool IsMouseButtonPressedImpl(MouseButton button) = 0;
    virtual glm::vec2 GetMousePositionImpl() = 0;
    virtual float GetMouseXImpl() = 0;
    virtual float GetMouseYImpl() = 0;
    
private:
    static Input* s_Instance;
};

} // namespace zrn