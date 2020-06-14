#pragma once

#include <glm/glm.hpp>

#include "Renderer/texture.h"

namespace de {
class Atlas2D {
public:
    Atlas2D(const char* atlasTexturePath, const glm::vec2& cellSize);

    [[nodiscard]] inline constexpr const Ref<Texture2D>& GetTexture() const { return m_texture; }

    [[nodiscard]] inline uint32_t GetWidth() const { return m_texture->GetWidth(); }

    [[nodiscard]] inline uint32_t GetHeight() const { return m_texture->GetHeight(); }

    [[nodiscard]] inline constexpr const glm::vec2& GetCellSize() const { return m_cellSize; }

    inline constexpr void SetCellSize(const glm::vec2& value) { m_cellSize = value; }

private:
    Ref<Texture2D> m_texture;
    glm::vec2 m_cellSize;
};

class SubTexture2D {
public:
    SubTexture2D(Ref<Atlas2D> atlas, const glm::vec2& min, const glm::vec2& max);

    /**
     * Getter for the coordinates to draw subTexture of atlas
     * @return Vec2 containing the coordinates of the subTexture in the atlas
     */
    [[nodiscard]] inline constexpr const glm::vec2* GetCoordinates() const { return m_textureCoords; }

    /**
     * Getter for the Atlas2D of SubTexture2D
     * @return Atlas2D to draw subTexture from
     */
    [[nodiscard]] inline constexpr const Ref<Atlas2D>& GetAtlas() const { return m_atlas; }

    /**
     * Getter for the texture used in atlas
     * @return Atlas Texture2D
     */
    [[nodiscard]] inline const Ref<Texture2D>& GetTexture() const { return m_atlas->GetTexture(); }

    /**
     * Creates a subTexture based on coords in spriteSheet/atlas. Defaults to a cell of one by one.
     * @param spriteSheet Atlas to be used to create subTexture
     * @param coords Vec2 containing the coords of desired sprite in atlas
     * @return A sprite/subTexture
     */
    static Ref<SubTexture2D> CreateSprite(const Ref<Atlas2D>& spriteSheet, const glm::vec2& coords);

    /**
     * Creates a subTexture based on coords in spriteSheet/atlas
     * @param spriteSheet Atlas to be used to create subTexture
     * @param coords Vec2 containing the coords of desired sprite in atlas
     * @param spriteSize Vec2 with the number of cells the sprite will contain
     * @return A sprite/subTexture
     */
    static Ref<SubTexture2D> CreateSprite(const Ref<Atlas2D>& spriteSheet, const glm::vec2& coords, const glm::vec2& spriteSize);

private:
    glm::vec2 m_textureCoords[4];
    Ref<Atlas2D> m_atlas;
};
}  // namespace de
