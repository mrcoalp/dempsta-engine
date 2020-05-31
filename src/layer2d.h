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

    // temp(mpinto): remove after proper 2d render implementation
    de::Ref<de::Shader> m_squareShader;
    de::Ref<de::VertexArray> m_squareVertexArray;
    glm::vec3 m_squareColor{0.0f, 0.0f, 0.2f};
};
