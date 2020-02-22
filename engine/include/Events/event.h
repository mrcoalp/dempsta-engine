#pragma  once

#include "pch.h"

namespace de {
    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseBtnPressed, MouseBtnReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None                = 0,
        ApplicationEvent    = 1u << 0u,
        InputEvent          = 1u << 1u,
        KeyboardEvent       = 1u << 2u,
        MouseEvent          = 1u << 3u,
        MouseBtnEvent       = 1u << 4u
    };

    class Event {
    public:
        /**
         * @brief Getter for the EventType of an Event.
         * @return The event type.
         */
        [[nodiscard]] virtual EventType GetEventType() const = 0;

        /**
         * @brief Getter for the event name.
         * @return Event name.
         */
        [[nodiscard]] virtual const char *GetName() const = 0;

        /**
         * @brief Getter for the category flags.
         * @return The category flags.
         */
        [[nodiscard]] virtual unsigned int GetCategoryFlags() const = 0;

        /**
         * @brief Use for debug only. Abstraction to better log.
         * @return A string version of the event.
         */
        [[nodiscard]] virtual std::string ToString() const {
            return GetName();
        }

        /**
         * @brief Checks if an event is the given category.
         * @param category
         * @return True if it is, false otherwise.
         */
        inline bool IsInCategory(EventCategory category) {
            return GetCategoryFlags() & category;
        }

    protected:
        bool handled = false;
    };

// Macros to avoid boilerplate in event classes
#define EVENT_CLASS_TYPE(type)  static EventType GetStaticEventType() { return EventType::type; }\
                                [[nodiscard]] virtual EventType GetEventType() const override { return GetStaticEventType(); }\
                                [[nodiscard]] virtual const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)  [[nodiscard]] virtual unsigned int GetCategoryFlags() const override { return category; }
}
