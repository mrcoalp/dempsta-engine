#pragma once

#include <dempsta.h>

class Layer2D : public de::Layer {
public:
    explicit Layer2D();

    void OnAttach() final;

    void OnDetach() final;

    void OnUpdate(const de::TimeStep& ts) final;

    void OnEvent(de::Event& e) final;

    void OnImGuiRender() final;

private:
    de::OrthographicCameraController m_cameraController;
    glm::vec4 m_squareColor{0.0f, 0.0f, 0.2f, 0.5f};
    de::Ref<de::Texture2D> m_texture;
};
