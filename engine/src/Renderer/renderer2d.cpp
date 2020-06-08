#include "Renderer/renderer2d.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/rendercommand.h"
#include "Renderer/shader.h"
#include "Renderer/vertexarray.h"

namespace de {
struct Renderer2DData {
    Ref<VertexArray> vertexArray;
    Ref<Shader> shader;
    Ref<Texture2D> whiteTextureRef;
};

static Renderer2DData data;

void Renderer2D::Init() {
    data.vertexArray = VertexArray::Create();

    float _squareVertices[5 * 4] = {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
                                    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f};

    auto _vertexBuffer = VertexBuffer::Create(_squareVertices, sizeof(_squareVertices));
    BufferLayout _layout = {{ShaderDataType::Vec3, "position"}, {ShaderDataType::Vec2, "texture"}};
    _vertexBuffer->SetLayout(_layout);
    data.vertexArray->AddVertexBuffer(_vertexBuffer);

    unsigned _squareIndices[6] = {0, 1, 2, 2, 3, 0};
    auto _indexBuffer = IndexBuffer::Create(_squareIndices, sizeof(_squareIndices) / sizeof(unsigned));
    data.vertexArray->AddIndexBuffer(_indexBuffer);

    data.whiteTextureRef = Texture2D::Create(1, 1);
    uint32_t _white = 0xffffffff;
    data.whiteTextureRef->SetData(&_white, sizeof(uint32_t));

    data.shader = Shader::Create("assets/shaders/quad.glsl");
    data.shader->Bind();
    data.shader->SetInt("u_texture", 0);
}

void Renderer2D::Shutdown() {}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    data.shader->Bind();
    data.shader->SetMat4("u_viewProjection", camera.GetProjectionViewMatrix());
}

void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    glm::mat4 _transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    data.shader->SetMat4("u_transform", _transform);
    data.shader->SetVec4("u_color", color);
    data.whiteTextureRef->Bind();
    data.vertexArray->Bind();
    RenderCommand::DrawIndexed(data.vertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
    DrawQuad({position.x, position.y, 0.0f}, size, texture);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
    DrawQuad(position, size, texture, glm::vec4(1.0f));
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                          const glm::vec4& tint) {
    DrawQuad({position.x, position.y, 0.0f}, size, texture, tint);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                          const glm::vec4& tint) {
    glm::mat4 _transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    data.shader->SetMat4("u_transform", _transform);
    data.shader->SetVec4("u_color", tint);
    texture->Bind();
    data.vertexArray->Bind();
    RenderCommand::DrawIndexed(data.vertexArray);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                 const glm::vec4& color) {
    DrawRotatedQuad(rotation, {position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                 const glm::vec4& color) {
    glm::mat4 _transform = glm::translate(glm::mat4(1.0f), position) *
                           glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                           glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    data.shader->SetMat4("u_transform", _transform);
    data.shader->SetVec4("u_color", color);
    data.whiteTextureRef->Bind();
    data.vertexArray->Bind();
    RenderCommand::DrawIndexed(data.vertexArray);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                 const Ref<Texture2D>& texture) {
    DrawRotatedQuad(rotation, {position.x, position.y, 0.0f}, size, texture);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                 const Ref<Texture2D>& texture) {
    DrawRotatedQuad(rotation, position, size, texture, glm::vec4(1.0f));
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                 const Ref<Texture2D>& texture, const glm::vec4& tint) {
    DrawRotatedQuad(rotation, {position.x, position.y, 0.0f}, size, texture, tint);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                 const Ref<Texture2D>& texture, const glm::vec4& tint) {
    glm::mat4 _transform = glm::translate(glm::mat4(1.0f), position) *
                           glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                           glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    data.shader->SetMat4("u_transform", _transform);
    data.shader->SetVec4("u_color", tint);
    texture->Bind();
    data.vertexArray->Bind();
    RenderCommand::DrawIndexed(data.vertexArray);
}
}  // namespace de
