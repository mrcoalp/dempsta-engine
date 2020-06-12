#include "Renderer/renderer2d.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/rendercommand.h"
#include "Renderer/shader.h"
#include "Renderer/vertexarray.h"

namespace de {
struct QuadVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texture;
    float textureIndex;
};

struct Renderer2DData {
    // Max batch per draw call
    static constexpr const uint32_t maxQuads = 10000;
    static constexpr const uint32_t maxVertices = maxQuads * 4;
    static constexpr const uint32_t maxIndices = maxQuads * 6;
    static constexpr const uint32_t maxTextureSlots = 32;  // TODO(MPINTO): query the GPU for this

    Ref<VertexBuffer> vertexBuffer;
    Ref<VertexArray> vertexArray;
    Ref<Shader> shader;
    Ref<Texture2D> whiteTextureRef;  // Used to render "no" textured quads

    uint32_t quadIndexCount = 0;
    QuadVertex* quadVertexBufferBase = nullptr;
    QuadVertex* quadVertexBufferPtr = nullptr;
    glm::vec4 quadVerticesPosition[4];  // Handle transformations

    std::array<Ref<Texture2D>, maxTextureSlots> textures;
    uint32_t textureSlotIndex = 1;  // 0 is reserved for our white texture ref

    Renderer2D::Statistics statistics;
};

static Renderer2DData data;

const Renderer2D::Statistics& Renderer2D::GetStatistics() { return data.statistics; }

void Renderer2D::ResetStatistics() {
    data.statistics.quads = 0;
    data.statistics.drawCalls = 0;
}

void Renderer2D::Init() {
    data.statistics.maxQuadsPerDraw = Renderer2DData::maxQuads;
    data.vertexArray = VertexArray::Create();

    data.vertexBuffer = VertexBuffer::Create(Renderer2DData::maxVertices * sizeof(QuadVertex));

    BufferLayout _layout = {{ShaderDataType::Vec3, "position"},
                            {ShaderDataType::Vec4, "color"},
                            {ShaderDataType::Vec2, "texture"},
                            {ShaderDataType::Float, "textureIndex"}};

    data.vertexBuffer->SetLayout(_layout);
    data.vertexArray->AddVertexBuffer(data.vertexBuffer);

    data.quadVertexBufferBase = new QuadVertex[Renderer2DData::maxVertices];

    auto* quadIndices = new uint32_t[Renderer2DData::maxIndices];
    constexpr const uint8_t indices[6] = {0, 1, 2, 2, 3, 0};
    uint32_t offset = 0;

    for (uint32_t i = 0; i < Renderer2DData::maxIndices; ++i) {
        const uint8_t indicesIndex = i % 6;
        quadIndices[i] = indices[indicesIndex] + offset;
        if (indicesIndex == 5) offset += 4;  // Next quad
    }

    auto _indexBuffer = IndexBuffer::Create(quadIndices, Renderer2DData::maxIndices);
    data.vertexArray->AddIndexBuffer(_indexBuffer);
    delete[] quadIndices;

    data.whiteTextureRef = Texture2D::Create(1, 1);
    uint32_t _white = 0xffffffff;
    data.whiteTextureRef->SetData(&_white, sizeof(uint32_t));

    int samplers[Renderer2DData::maxTextureSlots];
    for (uint32_t i = 0; i < Renderer2DData::maxTextureSlots; ++i) {
        samplers[i] = i;
    }

    data.shader = Shader::Create("assets/shaders/quad.glsl");
    data.shader->Bind();
    data.shader->SetIntArray("u_textures", samplers, Renderer2DData::maxTextureSlots);

    data.textures[0] = data.whiteTextureRef;

    data.quadVerticesPosition[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    data.quadVerticesPosition[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    data.quadVerticesPosition[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    data.quadVerticesPosition[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::Shutdown() {}

void Renderer2D::resetBuffer() {
    data.quadVertexBufferPtr = data.quadVertexBufferBase;
    data.quadIndexCount = 0;
    data.textureSlotIndex = 1;
}

void Renderer2D::checkDrawCall() {
    if (data.quadIndexCount >= Renderer2DData::maxIndices) {
        flush();
        resetBuffer();
    }
}

void Renderer2D::flush() {
    // Casting to a byte* gives the size in actual bytes
    uint32_t size = (uint8_t*)data.quadVertexBufferPtr - (uint8_t*)data.quadVertexBufferBase;
    data.vertexBuffer->SetData(data.quadVertexBufferBase, size);
    // Bind textures
    for (uint32_t i = 0; i < data.textureSlotIndex; ++i) {
        data.textures[i]->Bind(i);
    }
    RenderCommand::DrawIndexed(data.vertexArray, data.quadIndexCount);
    ++data.statistics.drawCalls;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    data.shader->Bind();
    data.shader->SetMat4("u_viewProjection", camera.GetProjectionViewMatrix());
    resetBuffer();
}

void Renderer2D::EndScene() { flush(); }

float Renderer2D::getOrAddUniqueTextureIndex(const Ref<Texture2D>& texture) {
    // Check if texture is already present and return index if so
    for (uint32_t i = 0; i < data.textureSlotIndex; ++i) {
        if (data.textures[i]->Equals(*texture)) {
            return (float)i;
        }
    }
    // Add texture to current index
    data.textures[data.textureSlotIndex] = texture;
    // return current index and increment after
    return (float)data.textureSlotIndex++;
}

void Renderer2D::DrawQuad(const Quad& quad) {
    checkDrawCall();

    float textureIndex = 0.0f;
    if (quad.texture) {
        textureIndex = getOrAddUniqueTextureIndex(quad.texture);
    } else if (quad.subTexture) {
        textureIndex = getOrAddUniqueTextureIndex(quad.subTexture->GetTexture());
    }

    glm::mat4 transform = quad.rotation != 0.0f
                              ? glm::translate(glm::mat4(1.0f), quad.position) *
                                    glm::rotate(glm::mat4(1.0f), glm::radians(quad.rotation), {0.0f, 0.0f, 1.0f}) *
                                    glm::scale(glm::mat4(1.0f), {quad.size.x, quad.size.y, 1.0f})
                              : glm::translate(glm::mat4(1.0f), quad.position) *
                                    glm::scale(glm::mat4(1.0f), {quad.size.x, quad.size.y, 1.0f});

    constexpr glm::vec2 coords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    for (uint8_t i = 0; i < 4; ++i) {
        data.quadVertexBufferPtr->position = transform * data.quadVerticesPosition[i];
        data.quadVertexBufferPtr->color = quad.tint;
        data.quadVertexBufferPtr->texture = quad.subTexture ? quad.subTexture->GetCoordinates()[i] : coords[i];
        data.quadVertexBufferPtr->textureIndex = textureIndex;
        ++data.quadVertexBufferPtr;
    }

    data.quadIndexCount += 6;

    ++data.statistics.quads;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    checkDrawCall();

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    constexpr const float x[4] = {0.0f, 1.0f, 1.0f, 0.0f};
    constexpr const float y[4] = {0.0f, 0.0f, 1.0f, 1.0f};

    for (uint8_t i = 0; i < 4; ++i) {
        data.quadVertexBufferPtr->position = transform * data.quadVerticesPosition[i];
        data.quadVertexBufferPtr->color = color;
        data.quadVertexBufferPtr->texture = {x[i], y[i]};
        data.quadVertexBufferPtr->textureIndex = 0.0f;
        ++data.quadVertexBufferPtr;
    }

    data.quadIndexCount += 6;

    ++data.statistics.quads;
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
    checkDrawCall();

    float textureIndex = getOrAddUniqueTextureIndex(texture);

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    constexpr const float x[4] = {0.0f, 1.0f, 1.0f, 0.0f};
    constexpr const float y[4] = {0.0f, 0.0f, 1.0f, 1.0f};

    for (uint8_t i = 0; i < 4; ++i) {
        data.quadVertexBufferPtr->position = transform * data.quadVerticesPosition[i];
        data.quadVertexBufferPtr->color = tint;
        data.quadVertexBufferPtr->texture = {x[i], y[i]};
        data.quadVertexBufferPtr->textureIndex = textureIndex;
        ++data.quadVertexBufferPtr;
    }

    data.quadIndexCount += 6;

    ++data.statistics.quads;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture) {
    DrawQuad({position.x, position.y, 0.0f}, size, subTexture);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture) {
    DrawQuad(position, size, subTexture, glm::vec4(1.0f));
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                          const glm::vec4& tint) {
    DrawQuad({position.x, position.y, 0.0f}, size, subTexture, tint);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                          const glm::vec4& tint) {
    checkDrawCall();

    float textureIndex = getOrAddUniqueTextureIndex(subTexture->GetTexture());

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    for (uint8_t i = 0; i < 4; ++i) {
        data.quadVertexBufferPtr->position = transform * data.quadVerticesPosition[i];
        data.quadVertexBufferPtr->color = tint;
        data.quadVertexBufferPtr->texture = subTexture->GetCoordinates()[i];
        data.quadVertexBufferPtr->textureIndex = textureIndex;
        ++data.quadVertexBufferPtr;
    }

    data.quadIndexCount += 6;

    ++data.statistics.quads;
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                 const glm::vec4& color) {
    DrawRotatedQuad(rotation, {position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                 const glm::vec4& color) {
    checkDrawCall();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    constexpr const float x[4] = {0.0f, 1.0f, 1.0f, 0.0f};
    constexpr const float y[4] = {0.0f, 0.0f, 1.0f, 1.0f};

    for (uint8_t i = 0; i < 4; ++i) {
        data.quadVertexBufferPtr->position = transform * data.quadVerticesPosition[i];
        data.quadVertexBufferPtr->color = color;
        data.quadVertexBufferPtr->texture = {x[i], y[i]};
        data.quadVertexBufferPtr->textureIndex = 0.0f;
        ++data.quadVertexBufferPtr;
    }

    data.quadIndexCount += 6;

    ++data.statistics.quads;
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
    checkDrawCall();

    float textureIndex = getOrAddUniqueTextureIndex(texture);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    constexpr const float x[4] = {0.0f, 1.0f, 1.0f, 0.0f};
    constexpr const float y[4] = {0.0f, 0.0f, 1.0f, 1.0f};

    for (uint8_t i = 0; i < 4; ++i) {
        data.quadVertexBufferPtr->position = transform * data.quadVerticesPosition[i];
        data.quadVertexBufferPtr->color = tint;
        data.quadVertexBufferPtr->texture = {x[i], y[i]};
        data.quadVertexBufferPtr->textureIndex = textureIndex;
        ++data.quadVertexBufferPtr;
    }

    data.quadIndexCount += 6;

    ++data.statistics.quads;
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                 const Ref<SubTexture2D>& subTexture) {
    DrawRotatedQuad(rotation, {position.x, position.y, 0.0f}, size, subTexture);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                 const Ref<SubTexture2D>& subTexture) {
    DrawRotatedQuad(rotation, position, size, subTexture, glm::vec4(1.0f));
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                 const Ref<SubTexture2D>& subTexture, const glm::vec4& tint) {
    DrawRotatedQuad(rotation, {position.x, position.y, 0.0f}, size, subTexture, tint);
}

void Renderer2D::DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                 const Ref<SubTexture2D>& subTexture, const glm::vec4& tint) {
    checkDrawCall();

    float textureIndex = getOrAddUniqueTextureIndex(subTexture->GetTexture());

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    for (uint8_t i = 0; i < 4; ++i) {
        data.quadVertexBufferPtr->position = transform * data.quadVerticesPosition[i];
        data.quadVertexBufferPtr->color = tint;
        data.quadVertexBufferPtr->texture = subTexture->GetCoordinates()[i];
        data.quadVertexBufferPtr->textureIndex = textureIndex;
        ++data.quadVertexBufferPtr;
    }

    data.quadIndexCount += 6;

    ++data.statistics.quads;
}
}  // namespace de
