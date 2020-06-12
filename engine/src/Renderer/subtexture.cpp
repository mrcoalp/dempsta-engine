#include "Renderer/subtexture.h"

#include <utility>

namespace de {
Atlas2D::Atlas2D(const char* atlasTexturePath, const glm::vec2& cellSize) : m_cellSize(cellSize) {
    m_texture = Texture2D::Create(atlasTexturePath);
}

SubTexture2D::SubTexture2D(Ref<Atlas2D> atlas, const glm::vec2& min, const glm::vec2& max) : m_atlas(std::move(atlas)) {
    m_textureCoords[0] = {min.x, min.y};
    m_textureCoords[1] = {max.x, min.y};
    m_textureCoords[2] = {max.x, max.y};
    m_textureCoords[3] = {min.x, max.y};
}

Ref<SubTexture2D> SubTexture2D::CreateSprite(const Ref<Atlas2D>& spriteSheet, const glm::vec2& coords) {
    return CreateSprite(spriteSheet, coords, {1.0f, 1.0f});
}

Ref<SubTexture2D> SubTexture2D::CreateSprite(const Ref<Atlas2D>& spriteSheet, const glm::vec2& coords, const glm::vec2& spriteSize) {
    const glm::vec2 min = {(coords.x * spriteSheet->GetCellSize().x) / (float)spriteSheet->GetWidth(),
                           (coords.y * spriteSheet->GetCellSize().y) / (float)spriteSheet->GetHeight()};
    const glm::vec2 max = {
        ((coords.x + spriteSize.x) * spriteSheet->GetCellSize().x) / (float)spriteSheet->GetWidth(),
        ((coords.y + spriteSize.y) * spriteSheet->GetCellSize().y) / (float)spriteSheet->GetHeight()};

    return CreateRef<SubTexture2D>(spriteSheet, min, max);
}
}  // namespace de
