#include "Scene/scenecamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace de {
SceneCamera::SceneCamera() { recalculateProjection(); }

void SceneCamera::SetOrthographic(float size, float near, float far) {
    m_orthoSize = size;
    m_orthoNear = near;
    m_orthoFar = far;
    recalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_aspectRatio = (float)width / (float)height;
    recalculateProjection();
}

void SceneCamera::recalculateProjection() {
    float left = -m_orthoSize * m_aspectRatio * 0.5f;
    float right = m_orthoSize * m_aspectRatio * 0.5f;
    float bottom = -m_orthoSize * 0.5f;
    float top = m_orthoSize * 0.5f;

    m_projection = glm::ortho(left, right, bottom, top, m_orthoNear, m_orthoFar);
}
}  // namespace de
