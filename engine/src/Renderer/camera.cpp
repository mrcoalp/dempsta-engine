#include "Renderer/camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace de {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f) {}

void OrthographicCamera::SetPosition(const glm::vec3& position) {
    m_position = position;
    updateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_rotation = rotation;
    updateViewMatrix();
}

void OrthographicCamera::updateViewMatrix() {
    glm::mat4 _transform = glm::translate(glm::mat4(1.0f), m_position) *
                           glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
    m_viewMatrix = glm::inverse(_transform);
}
}  // namespace de
