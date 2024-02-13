#pragma once

#include "Zarin/Input/Input.hpp"

namespace zrn {

// Input* Input::s_Instance = nullptr;

class WindowsInput : public Input {
protected:
    virtual bool IsKeyPressedImpl(KeyCode keycode) override;
    virtual bool IsMouseButtonPressedImpl(MouseButton keycode) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};

} // namespace zrn