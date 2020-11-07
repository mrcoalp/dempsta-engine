#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>

#include "Core/core.h"
#include "Renderer/texture.h"

namespace de {
struct FontCharacter {
    float advanceX;
    float advanceY;

    float bitmapWidth;
    float bitmapHeight;

    float bitmapLeft;
    float bitmapTop;

    float uvOffsetX0;
    float uvOffsetY0;
    float uvOffsetX1;
    float uvOffsetY1;
};

class FontTextureAtlas {
public:
    explicit FontTextureAtlas(const FT_Face& face);

    [[nodiscard]] inline const auto& GetCharacters() { return m_characters; }

    [[nodiscard]] inline const Ref<Texture2D>& GetTexture() const { return m_texture; }

private:
    unsigned m_width{0};
    unsigned m_height{0};
    std::unordered_map<char, FontCharacter> m_characters;
    Ref<Texture2D> m_texture{nullptr};
};

class Font {
public:
    Font(const FT_Library& library, const std::string& source, FT_Long faceIndex, unsigned size);

    [[nodiscard]] const Ref<FontTextureAtlas>& GetAtlas() const { return m_atlas; }

    [[nodiscard]] unsigned GetSize() const { return m_size; }

private:
    Ref<FontTextureAtlas> m_atlas{nullptr};
    FT_Face m_face;
    unsigned m_size{20};
};
}  // namespace de
