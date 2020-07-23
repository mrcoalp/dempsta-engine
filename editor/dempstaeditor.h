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
    bool m_editingMode = true;
    OrthographicCameraController m_cameraController;
    Ref<Atlas2D> m_spriteSheet;
    Ref<SubTexture2D> m_spriteBarrel;
    Ref<FrameBuffer> m_frameBuffer;
    TimeStep m_ts;
    float m_timeAccumulator = 0.0f;
    float m_fps = 0.0f;
    Ref<Scene> m_activeScene;
    Entity m_square;
};
}  // namespace de
