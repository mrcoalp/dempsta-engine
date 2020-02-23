#pragma once

namespace de {
    class KeyEvent : public Event {
    public:
        /**
         * @brief Getter for the key code.
         * @return Key code int.
         */
        [[nodiscard]] inline int GetKeyCode() const {
            return keyCode;
        }

        EVENT_CLASS_CATEGORY(KeyboardEventCategory | InputEventCategory)

    protected:
        explicit KeyEvent(int keyCode) : keyCode(keyCode) {};

        int keyCode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), repeatCount(repeatCount) {}

        /**
         * @brief Getter a key pressed repeat count.
         * @return The repeat count of a key pressed event.
         */
        inline int GetRepeatCount() {
            return repeatCount;
        }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
