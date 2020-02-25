#pragma once

namespace de {
    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}

        [[nodiscard]] inline float GetX() const { return mouseX; }

        [[nodiscard]] inline float GetY() const { return mouseY; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << "x: " << mouseX << ", y: " << mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)

        EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

    private:
        float mouseX;
        float mouseY;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float offsetX, float offsetY) : offsetX(offsetX), offsetY(offsetY) {}

        [[nodiscard]] inline float GetOffsetX() const { return offsetX; }

        [[nodiscard]] inline float GetOffsetY() const { return offsetY; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << "x: " << offsetX << ", y: " << offsetY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)

        EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

    private:
        float offsetX;
        float offsetY;
    };

    class MouseBtnEvent : public Event {
    public:
        [[nodiscard]] inline int GetMouseBtnCode() const { return mouseBtnCode; }

        EVENT_CLASS_CATEGORY(InputEventCategory | MouseEventCategory)

    protected:
        explicit MouseBtnEvent(int mouseBtnCode) : mouseBtnCode(mouseBtnCode) {}

        int mouseBtnCode;
    };

    class MouseBtnPressedEvent : public MouseBtnEvent {
    public:
        explicit MouseBtnPressedEvent(int mouseBtnCode) : MouseBtnEvent(mouseBtnCode) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseBtnPressedEvent: " << mouseBtnCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseBtnPressed)
    };

    class MouseBtnReleasedEvent : public MouseBtnEvent {
    public:
        explicit MouseBtnReleasedEvent(int mouseBtnCode) : MouseBtnEvent(mouseBtnCode) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseBtnReleasedEvent: " << mouseBtnCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseBtnReleased)
    };
}
