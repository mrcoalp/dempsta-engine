#pragma once

#include <glm/glm.hpp>

#include "Renderer/texture.h"

namespace de {
class SubTexture2D {
public:
    SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max);

    [[nodiscard]] inline constexpr const glm::vec2* GetCoordinates() const { return m_textureCoords; }

    [[nodiscard]] inline constexpr const Ref<Texture2D>& GetTexture() const { return m_texture; }

private:
    glm::vec2 m_textureCoords[4];
    Ref<Texture2D> m_texture;
};

class Atlas {
public:
    Atlas(const char* atlasTexturePath, const glm::vec2& cellSize);

    [[nodiscard]] inline constexpr const Ref<Texture2D>& GetTexture() const { return m_texture; }

    [[nodiscard]] inline uint32_t GetWidth() const { return m_texture->GetWidth(); }

    [[nodiscard]] inline uint32_t GetHeight() const { return m_texture->GetHeight(); }

    [[nodiscard]] inline constexpr const glm::vec2& GetCellSize() const { return m_cellSize; }

private:
    Ref<Texture2D> m_texture;
    glm::vec2 m_cellSize;
};

class Sprite : public SubTexture2D {
public:
    Sprite(const Ref<Atlas>& spriteSheet, const glm::vec2& coords, const glm::vec2& spriteSize = {1.0f, 1.0f});
};
}  // namespace de
