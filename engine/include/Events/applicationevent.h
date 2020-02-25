#pragma once

namespace de {
    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}

        /**
         * @brief Getter for the window width.
         * @return Width of window.
         */
        [[nodiscard]] inline unsigned int GetWidth() const { return width; }

        /**
         * @brief Getter for the window height.
         * @return Height of window.
         */
        [[nodiscard]] inline unsigned int GetHeight() const { return height; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << width << ", " << height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)

        EVENT_CLASS_CATEGORY(ApplicationEventCategory)

    private:
        unsigned int width;
        unsigned int height;
    };

    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)

        EVENT_CLASS_CATEGORY(ApplicationEventCategory)
    };

    class AppTickEvent : public Event {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)

        EVENT_CLASS_CATEGORY(ApplicationEventCategory)
    };

    class AppUpdateEvent : public Event {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)

        EVENT_CLASS_CATEGORY(ApplicationEventCategory)
    };

    class AppRenderEvent : public Event {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)

        EVENT_CLASS_CATEGORY(ApplicationEventCategory)
    };
}
