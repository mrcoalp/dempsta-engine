#pragma once

#include "Renderer/camera.h"

namespace de {
/**
 * @brief Represents the camera handled by scene.
 */
class SceneCamera : public Camera {
public:
    SceneCamera();

    /**
     * @brief Set the Orthographic properties.
     *
     * @param size Orthographic size.
     * @param near Orthographic near.
     * @param far Orthographic far.
     */
    void SetOrthographic(float size, float orthoNear, float orthoFar);

    /**
     * @brief Set the viewport size.
     *
     * @param width Width of viewport.
     * @param height Height of viewport.
     */
    void SetViewportSize(uint32_t width, uint32_t height);

    /**
     * @brief Getter for the orthographic size.
     *
     * @return Orthographic size.
     */
    [[nodiscard]] inline float GetOrthographicSize() const { return m_orthoSize; }

    /**
     * @brief Getter for the orthographic near clip.
     *
     * @return Orthographic near clip.
     */
    [[nodiscard]] inline float GetOrthographicNearClip() const { return m_orthoNear; }

    /**
     * @brief Getter for the orthographic far clip.
     *
     * @return Orthographic far clip.
     */
    [[nodiscard]] inline float GetOrthographicFarClip() const { return m_orthoFar; }

private:
    /**
     * @brief Recalculates projection based on updated properties.
     */
    void recalculateProjection();

    float m_aspectRatio{0.f};
    float m_orthoSize{10.f};
    float m_orthoNear{-1.f};
    float m_orthoFar{1.f};
};
}  // namespace de
