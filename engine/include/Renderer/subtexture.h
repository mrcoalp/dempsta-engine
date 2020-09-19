#pragma once

#include <glm/glm.hpp>

#include "Renderer/texture.h"

namespace de {
class Atlas2D {
public:
    Atlas2D(const std::string& atlasTexturePath, const glm::vec2& cellSize);

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

    explicit SubTexture2D(const std::string& texturePath);

    /**
     * @brief Getter for the coordinates to draw subTexture of atlas
     *
     * @return Vec2 containing the coordinates of the subTexture in the atlas
     */
    [[nodiscard]] inline constexpr const glm::vec2* GetCoordinates() const { return m_textureCoords; }

    /**
     * @brief Getter for the Atlas2D of SubTexture2D
     *
     * @return Atlas2D to draw subTexture from
     */
    [[nodiscard]] inline constexpr const Ref<Atlas2D>& GetAtlas() const { return m_atlas; }

    /**
     * @brief Getter for the texture used in atlas
     *
     * @return Atlas Texture2D
     */
    [[nodiscard]] inline const Ref<Texture2D>& GetTexture() const { return m_atlas->GetTexture(); }

    /**
     * @brief Getter for the subtexture anchor.
     * Each subtexture can have a different anchor, independent of the whole texture.
     *
     * @return const glm::vec2& anchor
     */
    [[nodiscard]] inline const glm::vec2& GetAnchor() const { return m_anchor; }

    /**
     * @brief Getter for the subtexture anchor.
     * Each subtexture can have a different anchor, independent of the whole texture.
     * (non-const)
     *
     * @return glm::vec2& anchor
     */
    inline glm::vec2& GetAnchor() { return m_anchor; }

    /**
     * @brief Sets the subtexture anchor.
     *
     * @param anchor A vec2 anchor to set.
     */
    void SetAnchor(const glm::vec2& anchor);

    /**
     * @brief Creates a subTexture based on coords in spriteSheet/atlas. Defaults to a cell of one by one.
     *
     * @param spriteSheet Atlas to be used to create subTexture
     * @param coords Vec2 containing the coords of desired sprite in atlas
     * @return A sprite/subTexture
     */
    static Ref<SubTexture2D> CreateSprite(const Ref<Atlas2D>& spriteSheet, const glm::vec2& coords);

    /**
     * @brief Creates a subTexture based on coords in spriteSheet/atlas
     *
     * @param spriteSheet Atlas to be used to create subTexture
     * @param coords Vec2 containing the coords of desired sprite in atlas
     * @param spriteSize Vec2 with the number of cells the sprite will contain
     * @return A sprite/subTexture
     */
    static Ref<SubTexture2D> CreateSprite(const Ref<Atlas2D>& spriteSheet, const glm::vec2& coords,
                                          const glm::vec2& spriteSize);

    /**
     * @brief Creates a sprite of entire texture in path.
     *
     * @param texturePath Path to the texture to create sprite from
     * @return A sprite/subTexture
     */
    static Ref<SubTexture2D> CreateSprite(const std::string& texturePath);

private:
    glm::vec2 m_textureCoords[4];
    Ref<Atlas2D> m_atlas;
    glm::vec2 m_anchor{0.f, 0.f};
};
}  // namespace de
