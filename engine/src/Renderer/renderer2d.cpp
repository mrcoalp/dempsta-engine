#include "Renderer/renderer2d.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/rendercommand.h"
#include "Renderer/shader.h"
#include "Renderer/vertexarray.h"

namespace de {
struct Renderer2DData {
    Ref<VertexArray> vertexArray;
    Ref<Shader> shader;
};

static Renderer2DData* data;

void Renderer2D::Init() {
    data = new Renderer2DData;
    data->vertexArray = VertexArray::Create();

    float _squareVertices[3 * 4] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f};

    auto _vertexBuffer = VertexBuffer::Create(_squareVertices, sizeof(_squareVertices));
    BufferLayout _layout = {{ShaderDataType::Vec3, "position"}};
    _vertexBuffer->SetLayout(_layout);
    data->vertexArray->AddVertexBuffer(_vertexBuffer);

    unsigned _squareIndices[6] = {0, 1, 2, 2, 3, 0};
    auto _indexBuffer = IndexBuffer::Create(_squareIndices, sizeof(_squareIndices) / sizeof(unsigned));
    data->vertexArray->AddIndexBuffer(_indexBuffer);

    data->shader = Shader::Create("assets/shaders/quad.glsl");
}

void Renderer2D::Shutdown() { delete data; }

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    data->shader->Bind();
    data->shader->SetMat4("u_viewProjection", camera.GetProjectionViewMatrix());
}

void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    data->shader->Bind();
    glm::mat4 _transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    data->shader->SetMat4("u_transform", _transform);
    data->shader->SetVec4("u_color", color);
    data->vertexArray->Bind();
    RenderCommand::DrawIndexed(data->vertexArray);
}
}  // namespace de
