#pragma once

#include "Events/event.h"

namespace de {
class KeyEvent : public Event {
public:
    /**
     * @brief Getter for the key code.
     * @return Key code int.
     */
    [[nodiscard]] inline int GetKeyCode() const { return m_keyCode; }

    EVENT_CLASS_CATEGORY(KeyboardEventCategory | InputEventCategory)

protected:
    explicit KeyEvent(int keyCode) : m_keyCode(keyCode){};

    int m_keyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_repeatCount(repeatCount) {}

    /**
     * @brief Getter a key pressed repeat count.
     * @return The repeat count of a key pressed event.
     */
    [[nodiscard]] inline int GetRepeatCount() const { return m_repeatCount; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    int m_repeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
    explicit KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_keyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
public:
    explicit KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_keyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};
}  // namespace de
