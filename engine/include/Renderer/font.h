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

    float uvOffsetX;
};

class FontTextureAtlas {
public:
    FontTextureAtlas(const FT_Face& face, unsigned height);

    [[nodiscard]] inline FontCharacter* GetCharacters() { return m_characters; }

    [[nodiscard]] inline const Ref<Texture2D>& GetTexture() const { return m_texture; }

private:
    unsigned m_width{0};
    unsigned m_height{0};
    FontCharacter m_characters[128];
    Ref<Texture2D> m_texture{nullptr};
};

class Font {
public:
    Font(const FT_Library& library, const std::string& source, FT_Long faceIndex, unsigned size);

    [[nodiscard]] const Ref<FontTextureAtlas>& GetAtlas() const { return m_atlas; }

private:
    Ref<FontTextureAtlas> m_atlas{nullptr};
    FT_Face m_face;
};

class FontManager {
public:
    static FontManager& GetInstance();

    FontManager(const FontManager&) = delete;

    void operator=(const FontManager&) = delete;

    void InitFreeType();

    void AddFont(const std::string& name, const std::string& source, unsigned size);

    [[nodiscard]] inline const Font& GetFont(const std::string& fontName) const { return m_fonts.at(fontName); }

private:
    FontManager() {}

    FT_Library m_library;
    FT_Long m_index{0};

    std::unordered_map<std::string, Font> m_fonts;
};
}  // namespace de
