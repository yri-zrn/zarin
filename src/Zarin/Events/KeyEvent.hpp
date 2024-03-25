#pragma once

#include "Events/Event.hpp"
#include "Input/KeyCodes.hpp"

#include <sstream>

namespace zrn {

class KeyEvent : public Event {
public:
    inline KeyCode GetKeyCode() const { return m_KeyCode; }
    inline KeyMod GetKeyMod() const { return m_KeyMod; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    KeyEvent(KeyCode keycode, KeyMod mod)
    : m_KeyCode(keycode), m_KeyMod(mod) { }

    KeyCode m_KeyCode;
    KeyMod m_KeyMod;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(KeyCode keycode, KeyMod mod, int repeat_count)
    : KeyEvent(keycode, mod), m_RepeatCount(repeat_count) { }

    inline int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressed: " << m_KeyCode << " (" << m_KeyMod << " mod, " << m_RepeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(KeyCode keycode, KeyMod mod)
    : KeyEvent(keycode, mod) { }
    
    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleased: " << m_KeyCode << " (" << m_KeyMod << " mod)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

} // namespace zrn