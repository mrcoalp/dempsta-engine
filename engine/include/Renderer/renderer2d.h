#pragma once

#include "Renderer/camera.h"
#include "Renderer/texture.h"

namespace de {
class Renderer2D {
public:
    static void Init();

    static void Shutdown();

    static void BeginScene(const OrthographicCamera& camera);

    static void EndScene();

    // Primitives
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint);

    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
    static void DrawRotatedQuad(float rotation, const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint);
    static void DrawRotatedQuad(float rotation, const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint);

    struct Statistics {
        uint32_t drawCalls = 0;
        uint32_t quads = 0;
        uint32_t GetDrawnVertices() { return quads * 4; }
        uint32_t GetDrawnIndices() { return quads * 6; }
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
