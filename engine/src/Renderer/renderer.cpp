#include "Renderer/renderer.h"

#include "Platform/OpenGL/openglshader.h"

namespace de {
Ref<Renderer::SceneData> Renderer::s_sceneData = std::make_shared<Renderer::SceneData>();

void Renderer::Init() {
    RenderCommand::Init();
}

void Renderer::OnWindowResize(unsigned int width, unsigned int height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera& camera) {
    s_sceneData->projectionViewMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_viewProjection",
                                                                       s_sceneData->projectionViewMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_transform", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}
}  // namespace de
