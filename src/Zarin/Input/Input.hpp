#pragma once

#include "Core/Core.hpp"

#include "Input/KeyCodes.hpp"
#include "Input/MouseCodes.hpp"

#include <glm/vec2.hpp>

namespace zrn {

class Input {
public:
    static bool IsKeyPressed(KeyCode keycode);

    static bool IsMouseButtonPressed(MouseButton button);
    static glm::vec2 GetMousePosition();
    // static glm::vec2 GetMouseDelta(); // probably in the future
    static float GetMouseX();
    static float GetMouseY();

    // static void HideCursor();
    // static void ShowCursor();
};

} // namespace zrn