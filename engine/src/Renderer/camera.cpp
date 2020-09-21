// Avoid min max windows definition
#define NOMINMAX

#include "Renderer/camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Input/input.h"
#include "Input/keycodes.h"
#include "Input/mousebuttoncodes.h"

namespace de {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f) {}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
    m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

void OrthographicCamera::SetPosition(const glm::vec3& position) {
    m_position = position;
    updateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_rotation = rotation;
    updateViewMatrix();
}

void OrthographicCamera::updateViewMatrix() {
    glm::mat4 _transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
    m_viewMatrix = glm::inverse(_transform);
}

OrthographicCameraController::OrthographicCameraController(float aspectRatio)
    : m_aspectRatio(aspectRatio),
      m_bounds({-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel}),
      m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top),
      m_rotation(false) {}

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_aspectRatio(aspectRatio),
      m_bounds({-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel}),
      m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top),
      m_rotation(rotation) {}

void OrthographicCameraController::OnUpdate(const TimeStep& ts) {
#ifdef NDEBUG
    static glm::vec3 movementOffset = {0.0f, 0.0f, 0.0f};

    if (Input::IsMouseButtonPressed(DE_MOUSE_BUTTON_LEFT) && Input::IsKeyPressed(DE_KEY_LEFT_CONTROL)) {
        if (m_previousMousePosition == std::pair<float, float>(0.0f, 0.0f)) {
            m_previousMousePosition = Input::GetMousePosition();
        }
        movementOffset = {m_previousMousePosition.first - Input::GetMouseX(), Input::GetMouseY() - m_previousMousePosition.second, 0.0f};
        m_previousMousePosition = Input::GetMousePosition();
        glm::vec3 _position = m_camera.GetPosition() + (m_zoomLevel * (float)ts * movementOffset * 0.2f);
        m_camera.SetPosition(_position);
    } else {
        m_previousMousePosition = {0.0f, 0.0f};
    }

    if (m_rotation) {
        if (Input::IsKeyPressed(DE_KEY_Q) && Input::IsKeyPressed(DE_KEY_LEFT_CONTROL)) {
            float _rotation = m_camera.GetRotation() + 180.0f * (float)ts;
            m_camera.SetRotation(_rotation);
        }
        if (Input::IsKeyPressed(DE_KEY_E) && Input::IsKeyPressed(DE_KEY_LEFT_CONTROL)) {
            float _rotation = m_camera.GetRotation() - 180.0f * (float)ts;
            m_camera.SetRotation(_rotation);
        }
    }
#endif
}

void OrthographicCameraController::OnEvent(Event& e) {
    EventDispatcher _dispatcher(e);
    _dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) { return onMouseScrolled(event); });
    _dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) { return onWindowResized(event); });
}

void OrthographicCameraController::OnResize(uint32_t width, uint32_t height) {
    if (height == 0) {
        LOG_ENGINE_WARN("Tried to resize camera with an invalid height");
        return;
    }
    m_aspectRatio = (float)width / (float)height;
    m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
}

bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
    m_zoomLevel -= e.GetOffsetY() * 0.25f;
    m_zoomLevel = std::max(m_zoomLevel, 0.25f);
    m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
    return false;
}

bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
    // Avoid seg fault on windows that sets width and height to 0 when minimized window
    if (e.GetHeight() == 0) {
        return false;
    }
    OnResize(e.GetWidth(), e.GetHeight());
    return false;
}
}  // namespace de
