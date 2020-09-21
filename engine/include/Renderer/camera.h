#pragma once

#include <glm/glm.hpp>

#include "Core/timestep.h"
#include "Events/applicationevent.h"
#include "Events/mouseevent.h"

namespace de {
class Camera {
public:
    Camera() = default;

    explicit Camera(const glm::mat4& projection) : m_projection(projection) {}

    virtual ~Camera() = default;

    [[nodiscard]] const glm::mat4& GetProjection() const { return m_projection; }

protected:
    glm::mat4 m_projection{glm::mat4(1.f)};
};

struct OrthographicCameraBounds {
    float left, right, bottom, top;
    [[nodiscard]] inline float GetWidth() const { return right - left; }
    [[nodiscard]] inline float GetHeight() const { return top - bottom; }
};

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

    [[nodiscard]] inline float GetAspectRatio() const { return m_aspectRatio; }

    [[nodiscard]] inline float GetZoomLevel() const { return m_zoomLevel; }

    [[nodiscard]] inline const OrthographicCameraBounds& GetBounds() const { return m_bounds; }

    inline void SetZoomLevel(float value) { m_zoomLevel = value; }

    void OnResize(uint32_t width, uint32_t height);

private:
    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onWindowResized(WindowResizeEvent& e);

    float m_aspectRatio;
    float m_zoomLevel = 1.0f;
    OrthographicCameraBounds m_bounds;
    OrthographicCamera m_camera;
    bool m_rotation;
#ifdef NDEBUG
    std::pair<float, float> m_previousMousePosition;
#endif
};
}  // namespace de
