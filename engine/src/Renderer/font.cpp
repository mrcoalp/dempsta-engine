#include "Renderer/font.h"

#include <algorithm>

#include "Renderer/rendercommand.h"

namespace de {
FontTextureAtlas::FontTextureAtlas(const FT_Face& face) {
    FT_GlyphSlot glyph = face->glyph;
    const uint32_t maxTextureSize = RenderCommand::GetMaxTextureSize();
    int roww = 0;
    int rowh = 0;
    // NOTE(mpinto): We skip the first 32 ASCII characters, since they are just control codes.
    for (uint16_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) > FT_Err_Ok) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }
        if (roww + glyph->bitmap.width + 1 >= maxTextureSize) {
            m_width = std::fmax(m_width, roww);
            m_height += rowh;
            roww = 0;
            rowh = 0;
        }
        roww += glyph->bitmap.width + 1;
        rowh = std::fmax(rowh, glyph->bitmap.rows);
    }

    m_width = std::fmax(m_width, roww);
    m_height += rowh;

    m_texture = Texture2D::Create(m_width, m_height);

    int x = 0;
    int y = 0;
    for (uint16_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) > FT_Err_Ok) {
            continue;
        }

        if (x + glyph->bitmap.width + 1 >= maxTextureSize) {
            y += rowh;
            rowh = 0;
            x = 0;
        }

        m_texture->SetData(glyph->bitmap.buffer, {x, y}, glyph->bitmap.width, glyph->bitmap.rows);

        FontCharacter character = {static_cast<float>(glyph->advance.x >> 6),
                                   static_cast<float>(glyph->advance.y >> 6),
                                   static_cast<float>(glyph->bitmap.width),
                                   static_cast<float>(glyph->bitmap.rows),
                                   static_cast<float>(glyph->bitmap_left),
                                   static_cast<float>(glyph->bitmap_top),
                                   static_cast<float>(x) / static_cast<float>(m_width),
                                   static_cast<float>(y) / static_cast<float>(m_height)};

        m_characters.emplace(i, character);

        rowh = std::fmax(rowh, glyph->bitmap.rows);
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
    m_atlas = CreateRef<FontTextureAtlas>(m_face);
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
