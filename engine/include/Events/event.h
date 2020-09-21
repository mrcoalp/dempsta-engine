#pragma once

#include <sstream>

#include "Core/log.h"

namespace de {
/**
 * @brief Event type enumeration.
 */
enum class EventType {
    None = 0,
    WindowClose,
    WindowResize,
    WindowIconify,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseBtnPressed,
    MouseBtnReleased,
    MouseMoved,
    MouseScrolled
};

/**
 * @brief Event category is represented as a bitfield.
 * An event can fall into multiple categories.
 */
enum EventCategory {
    None = 0,
    ApplicationEventCategory = 1u << 0u,
    InputEventCategory = 1u << 1u,
    KeyboardEventCategory = 1u << 2u,
    MouseEventCategory = 1u << 3u
};

/**
 * @brief Base class for all events.
 */
class Event {
public:
    virtual ~Event() = default;

    /**
     * @brief Has event been handled?
     */
    bool Handled = false;

    /**
     * @brief Getter for the EventType of an Event.
     * @return The event type.
     */
    [[nodiscard]] virtual EventType GetEventType() const = 0;

    /**
     * @brief Getter for the event name.
     * @return Event name.
     */
    [[nodiscard]] virtual const char* GetName() const = 0;

    /**
     * @brief Getter for the category flags.
     * @return The category flags.
     */
    [[nodiscard]] virtual unsigned int GetCategoryFlags() const = 0;

    /**
     * @brief Use for debug only. Abstraction to better log.
     * @return A string version of the event.
     */
    [[nodiscard]] virtual std::string ToString() const { return GetName(); }

    /**
     * @brief Checks if an event is the given category.
     * @param category
     * @return True if it is, false otherwise.
     */
    [[nodiscard]] inline bool IsInCategory(EventCategory category) const { return (GetCategoryFlags() & category) != 0; }
};

/**
 * @brief Class handler for dispatch an event.
 */
class EventDispatcher {
public:
    explicit EventDispatcher(Event& event) : event(event) {}

    /**
     * @brief Dispatcher method.
     * @tparam E Event to compare to.
     * @tparam F Lambda return type.
     * @param func Lambda to be used to get handled.
     * @return True if event was dispatched, false otherwise.
     */
    template <typename E, typename F>
    bool Dispatch(const F& func) {
        if (event.GetEventType() == E::GetStaticEventType()) {
            event.Handled = func(static_cast<E&>(event));
            return true;
        }
        return false;
    }

private:
    Event& event;
};

/**
 * @brief Overload of << operator to print event.ToString();
 * @param os
 * @param e
 * @return
 */
inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

// Macros to avoid boilerplate in event classes
#define EVENT_CLASS_TYPE(type)                                                                     \
    static EventType GetStaticEventType() { return EventType::type; }                              \
    [[nodiscard]] virtual EventType GetEventType() const override { return GetStaticEventType(); } \
    [[nodiscard]] virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    [[nodiscard]] virtual unsigned int GetCategoryFlags() const override { return category; }
}  // namespace de
