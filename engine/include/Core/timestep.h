#pragma once

namespace de {
struct TimeStep {
    explicit TimeStep(float time = 0.0f) : m_time(time) {}

    /**
     * Getter for the time in seconds.
     * @returns Time in seconds, as float.
     */
    [[nodiscard]] float GetSeconds() const { return m_time; }

    /**
     * Getter for the time in ms.
     * @returns Time in ms, as float.
     */
    [[nodiscard]] float GetMilliSeconds() const { return m_time * 1000.0f; }

    /** Cast to float to get time in seconds. */
    explicit operator float() const { return m_time; }

private:
    float m_time;
};
}  // namespace de
