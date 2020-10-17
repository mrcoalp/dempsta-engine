#pragma once

#include "Events/event.h"

namespace de {
class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}

    /**
     * @brief Getter for the window width.
     * @return Width of window.
     */
    [[nodiscard]] inline unsigned int GetWidth() const { return m_width; }

    /**
     * @brief Getter for the window height.
     * @return Height of window.
     */
    [[nodiscard]] inline unsigned int GetHeight() const { return m_height; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_width << ", " << m_height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)

    EVENT_CLASS_CATEGORY(ApplicationEventCategory)

private:
    unsigned int m_width;
    unsigned int m_height;
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)

    EVENT_CLASS_CATEGORY(ApplicationEventCategory)
};

class WindowIconifyEvent : public Event {
public:
    explicit WindowIconifyEvent(bool iconify) : m_iconify(iconify) {}

    /**
     * Getter for the iconify state.
     * @return true if iconify, false otherwise.
     */
    [[nodiscard]] inline bool GetIconify() const { return m_iconify; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowIconifyEvent: " << std::boolalpha << m_iconify;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowIconify)

    EVENT_CLASS_CATEGORY(ApplicationEventCategory)

private:
    bool m_iconify;
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
}  // namespace de
