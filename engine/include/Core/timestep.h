#pragma once

namespace de {
struct TimeStep {
    explicit TimeStep(float time = 0.0f) : m_time(time) {}

    [[nodiscard]] float GetSeconds() const { return m_time; }

    [[nodiscard]] float GetMilliSeconds() const { return m_time * 1000.0f; }

    explicit operator float() const { return m_time; }

private:
    float m_time;
};
}  // namespace de
