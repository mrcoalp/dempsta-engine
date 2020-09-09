#pragma once

#include "Renderer/camera.h"
#include "Renderer/label.h"
#include "Renderer/subtexture.h"
#include "Renderer/texture.h"

namespace de {
struct Quad {
    glm::vec3 position;
    glm::vec2 size;
    glm::vec4 tint = glm::vec4(1.0f);
    Ref<Texture2D> texture;
    Ref<SubTexture2D> subTexture;
    float rotation = 0.0f;
};

class Renderer2D {
public:
    static void Init();

    static void Shutdown();

    static void BeginScene(const glm::mat4& projection, const glm::mat4& transform);

    static void BeginScene(const OrthographicCamera& camera);

    static void EndScene();

    // Primitives
    static void DrawQuad(const Quad& quad);

    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
    static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
                         const glm::vec4& tint = glm::vec4(1.0f));
    static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture,
                         const glm::vec4& tint = glm::vec4(1.0f));
    static void DrawQuad(const glm::mat4& transform, const Ref<Label>& text, const glm::vec4& tint = glm::vec4(1.0f));

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                         const glm::vec4& tint);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                         const glm::vec4& tint);

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                         const glm::vec4& tint);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                         const glm::vec4& tint);

    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                const glm::vec4& color);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                const glm::vec4& color);

    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                const Ref<Texture2D>& texture);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                const Ref<Texture2D>& texture);
    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                const Ref<Texture2D>& texture, const glm::vec4& tint);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                const Ref<Texture2D>& texture, const glm::vec4& tint);

    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                const Ref<SubTexture2D>& subTexture);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                const Ref<SubTexture2D>& subTexture);
    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size,
                                const Ref<SubTexture2D>& subTexture, const glm::vec4& tint);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size,
                                const Ref<SubTexture2D>& subTexture, const glm::vec4& tint);

    struct Statistics {
        uint32_t maxQuadsPerDraw = 0;
        uint32_t drawCalls = 0;
        uint32_t quads = 0;
        [[nodiscard]] uint32_t GetDrawnVertices() const { return quads * 4; }
        [[nodiscard]] uint32_t GetDrawnIndices() const { return quads * 6; }
    };
    static const Statistics& GetStatistics();
    static void ResetStatistics();

private:
    static void resetBuffer();
    static void flush();
    static void checkDrawCall();
    static float getOrAddUniqueTextureIndex(const Ref<Texture2D>& texture);
};
}  // namespace de
