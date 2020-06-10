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

    static void Flush();

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
};
}  // namespace de
