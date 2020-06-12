#pragma once

#include <dempsta.h>

class GameLayer : public de::Layer {
public:
    explicit GameLayer();

    void OnAttach() final;

    void OnDetach() final;

    void OnUpdate(const de::TimeStep& ts) final;

    void OnEvent(de::Event& e) final;

    void OnImGuiRender() final;

private:
    de::OrthographicCameraController m_cameraController;
    de::Ref<de::Atlas> m_spriteSheet;
    de::Ref<de::Sprite> m_sprite;
};
