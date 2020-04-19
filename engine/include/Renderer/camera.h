#pragma once

#include <glm/glm.hpp>

namespace de {
class OrthographicCamera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    [[nodiscard]] inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }

    [[nodiscard]] inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }

    [[nodiscard]] inline const glm::vec3& GetPosition() const { return m_position; }

    [[nodiscard]] inline float GetRotation() const { return m_rotation; }

    void SetPosition(const glm::vec3& position);

    void SetRotation(float rotation);

private:
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    float m_rotation{0.0f};

    void updateViewMatrix();
};
}  // namespace de
