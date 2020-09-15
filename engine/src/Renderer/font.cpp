#include "Renderer/font.h"

#include <algorithm>

namespace de {
FontTextureAtlas::FontTextureAtlas(const FT_Face& face, unsigned height) {
    FT_GlyphSlot glyph = face->glyph;
    // NOTE(mpinto): We skip the first 32 ASCII characters, since they are just control codes.
    for (uint16_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) > FT_Err_Ok) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }
        m_width += glyph->bitmap.width;
        m_height = std::max(height, glyph->bitmap.rows);
    }

    m_texture = Texture2D::Create(m_width, m_height);

    int x = 0;
    for (uint16_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) > FT_Err_Ok) {
            continue;
        }
        // TODO(mpinto): Check for max texture dimensions to also use rows and not only columns
        m_texture->SetData(glyph->bitmap.buffer, {x, 0}, glyph->bitmap.width, glyph->bitmap.rows);

        FontCharacter character = {glyph->advance.x >> 6, glyph->advance.y >> 6, glyph->bitmap.width,
                                   glyph->bitmap.rows,    glyph->bitmap_left,    glyph->bitmap_top,
                                   (float)x / m_width};

        m_characters.emplace(i, character);

        x += glyph->bitmap.width + 1;
    }

    LOG_ENGINE_TRACE("Generated a {} x {} ({} kb) texture atlas for font: {} - {}", m_width, m_height,
                     m_width * m_height / 1024, face->family_name, face->style_name);
}

Font::Font(const FT_Library& library, const std::string& source, FT_Long faceIndex, unsigned size) : m_size(size) {
    auto error = FT_New_Face(library, source.c_str(), faceIndex, &m_face);
    if (error == FT_Err_Unknown_File_Format) {
        LOG_ENGINE_ERROR("Font file could be opened and read, but it appears that its font format is unsupported");
    } else if (error > FT_Err_Ok) {
        LOG_ENGINE_ERROR("Font file could not be opened or read. Or it's broken.");
    }
    FT_Set_Pixel_Sizes(m_face, 0, size);
    m_atlas = CreateRef<FontTextureAtlas>(m_face, size);
}

FontManager& FontManager::GetInstance() {
    static FontManager instance;
    return instance;
}

void FontManager::InitFreeType() {
    if (FT_Init_FreeType(&m_library) > FT_Err_Ok) {
        LOG_ENGINE_ERROR("An error occurred during FreeType library initialization!");
    }
}

void FontManager::AddFont(const std::string& name, const std::string& source, unsigned size) {
    Font font(m_library, source, m_index++, size);
    m_fonts.emplace(name, font);
}
}  // namespace de
