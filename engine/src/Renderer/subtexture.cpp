#include "Renderer/subtexture.h"

#include <utility>

namespace de {

SubTexture2D::SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max)
    : m_texture(std::move(texture)) {
    m_textureCoords[0] = {min.x, min.y};
    m_textureCoords[1] = {max.x, min.y};
    m_textureCoords[2] = {max.x, max.y};
    m_textureCoords[3] = {min.x, max.y};
}

Sprite::Sprite(const Ref<Atlas>& spriteSheet, const glm::vec2& coords, const glm::vec2& spriteSize)
    : SubTexture2D(spriteSheet->GetTexture(),
                   {(coords.x * spriteSheet->GetCellSize().x) / (float)spriteSheet->GetWidth(),
                    (coords.y * spriteSheet->GetCellSize().y) / (float)spriteSheet->GetHeight()},
                   {((coords.x + spriteSize.x) * spriteSheet->GetCellSize().x) / (float)spriteSheet->GetWidth(),
                    ((coords.y + spriteSize.y) * spriteSheet->GetCellSize().y) / (float)spriteSheet->GetHeight()}) {}

Atlas::Atlas(const char* atlasTexturePath, const glm::vec2& cellSize) : m_cellSize(cellSize) {
    m_texture = Texture2D::Create(atlasTexturePath);
}
}  // namespace de
