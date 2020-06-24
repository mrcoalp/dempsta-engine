#pragma once

#include <dempsta.h>

class DempstaEditor : public de::Layer {
public:
    explicit DempstaEditor();

    void OnAttach() final;

    void OnDetach() final;

    void OnUpdate(const de::TimeStep& ts) final;

    void OnEvent(de::Event& e) final;

    void OnImGuiRender() final;

private:
    bool m_editingMode = true;
    de::OrthographicCameraController m_cameraController;
    de::Ref<de::Atlas2D> m_spriteSheet;
    de::Ref<de::SubTexture2D> m_spriteTree;
    de::Ref<de::SubTexture2D> m_spriteBarrel;
    de::Quad m_quad;
    de::Ref<de::FrameBuffer> m_frameBuffer;
    de::TimeStep m_ts;
    float m_timeAccumulator = 0.0f;
    float m_fps = 0.0f;
};
