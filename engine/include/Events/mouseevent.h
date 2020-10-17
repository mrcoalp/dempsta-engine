#pragma once

#include "Events/event.h"

namespace de {
class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

    [[nodiscard]] inline float GetX() const { return m_mouseX; }

    [[nodiscard]] inline float GetY() const { return m_mouseY; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: "
           << "x: " << m_mouseX << ", y: " << m_mouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)

    EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

private:
    float m_mouseX;
    float m_mouseY;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float offsetX, float offsetY) : m_offsetX(offsetX), m_offsetY(offsetY) {}

    [[nodiscard]] inline float GetOffsetX() const { return m_offsetX; }

    [[nodiscard]] inline float GetOffsetY() const { return m_offsetY; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: "
           << "x: " << m_offsetX << ", y: " << m_offsetY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)

    EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

private:
    float m_offsetX;
    float m_offsetY;
};

class MouseBtnEvent : public Event {
public:
    [[nodiscard]] inline int GetMouseBtnCode() const { return m_mouseBtnCode; }

    EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

protected:
    explicit MouseBtnEvent(int mouseBtnCode) : m_mouseBtnCode(mouseBtnCode) {}

    int m_mouseBtnCode;
};

class MouseBtnPressedEvent : public MouseBtnEvent {
public:
    explicit MouseBtnPressedEvent(int mouseBtnCode) : MouseBtnEvent(mouseBtnCode) {}

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseBtnPressedEvent: " << m_mouseBtnCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseBtnPressed)
};

class MouseBtnReleasedEvent : public MouseBtnEvent {
public:
    explicit MouseBtnReleasedEvent(int mouseBtnCode) : MouseBtnEvent(mouseBtnCode) {}

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseBtnReleasedEvent: " << m_mouseBtnCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseBtnReleased)
};
}  // namespace de
