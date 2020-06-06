#pragma once

#include <glm/glm.hpp>

#include "Core/timestep.h"
#include "Events/applicationevent.h"
#include "Events/mouseevent.h"

namespace de {
class OrthographicCamera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);

    [[nodiscard]] inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }

    [[nodiscard]] inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }

    [[nodiscard]] inline glm::mat4 GetProjectionViewMatrix() const { return m_projectionMatrix * m_viewMatrix; }

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

class OrthographicCameraController {
public:
    explicit OrthographicCameraController(float aspectRatio);
    OrthographicCameraController(float aspectRatio, bool rotation);

    void OnUpdate(const TimeStep& ts);

    void OnEvent(Event& e);

    [[nodiscard]] inline const OrthographicCamera& GetCamera() const { return m_camera; }

    [[nodiscard]] inline OrthographicCamera& GetCamera() { return m_camera; }

private:
    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onWindowResized(WindowResizeEvent& e);

    float m_aspectRatio;
    float m_zoomLevel = 1.0f;
    bool m_rotation;
    OrthographicCamera m_camera;
};
}  // namespace de
