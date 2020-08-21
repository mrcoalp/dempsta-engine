#pragma once

#include <dempsta.h>

namespace de {
class DempstaEditor : public Layer {
public:
    explicit DempstaEditor();

    void OnAttach() final;

    void OnDetach() final;

    void OnUpdate(const TimeStep& ts) final;

    void OnEvent(Event& e) final;

    void OnImGuiRender() final;

private:
    OrthographicCameraController m_cameraController;
    Ref<Atlas2D> m_spriteSheet;
    Ref<SubTexture2D> m_spriteBarrel;
    Ref<FrameBuffer> m_frameBuffer;
    TimeStep m_ts;
    float m_timeAccumulator{0.f};
    float m_fps{0.f};
    Ref<Scene> m_activeScene;
    Entity m_square;

    glm::vec2 m_viewportSize{glm::vec2(0.f)};
};
}  // namespace de
