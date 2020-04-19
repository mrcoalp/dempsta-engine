#include "Renderer/renderer.h"

namespace de {
Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData();

void Renderer::BeginScene(OrthographicCamera& camera) {
    s_sceneData->projectionViewMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& transform) {
    shader->Bind();
    shader->UploadUniformMat4("u_viewProjection", s_sceneData->projectionViewMatrix);
    shader->UploadUniformMat4("u_transform", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}
}  // namespace de
