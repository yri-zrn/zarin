#pragma once

#include <stdint.h>
#include <iostream>

namespace zrn {

// from SDL_mouse.h

typedef enum class MouseButton : uint32_t {
    Unknown = 0,
    Left    = 1,
    Middle  = 2,
    Right   = 3,
    X1      = 4,
    X2      = 5
} Button;

enum class CursorMode {
    Normal,
    Hidden,
    Locked
};

inline std::ostream& operator<<(std::ostream& os, MouseButton button) {
    os << static_cast<int32_t>(button);
    return os;
}

} // namespace zrn