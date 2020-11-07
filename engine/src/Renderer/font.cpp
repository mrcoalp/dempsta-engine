#include "Renderer/font.h"

#include <algorithm>

#include "Renderer/rendercommand.h"

namespace de {
FontTextureAtlas::FontTextureAtlas(const FT_Face& face) {
    FT_GlyphSlot glyph = face->glyph;
    const uint32_t maxTextureSize = RenderCommand::GetMaxTextureSize();
    uint32_t roww = 0;
    uint32_t rowh = 0;
    // NOTE(mpinto): We skip the first 32 ASCII characters, since they are just control codes.
    for (uint32_t i = 32; i < face->num_glyphs; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) > FT_Err_Ok) {
            LOG_ENGINE_WARN("Loading character {} failed!", i);
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

    uint32_t x = 0;
    uint32_t y = 0;
    for (uint32_t i = 32; i < face->num_glyphs; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER) > FT_Err_Ok) {
            continue;
        }

        if (x + glyph->bitmap.width + 1 >= maxTextureSize) {
            y += rowh;
            rowh = 0;
            x = 0;
        }

        {
            // NOTE(mpinto): Since freetype glyphs are only rendered to one channel, we need to expand it's data
            // Expanded data will be rendered with all 4 channels, so we add RGB (all 255(white)) and define alpha with proper data
            // This enables the use of all purpose shader in batch renderer
            const auto width = glyph->bitmap.width;
            const auto height = glyph->bitmap.rows;
            auto* data = new unsigned char[4 * width * height];

            for (uint32_t h = 0; h < height; ++h) {
                for (uint32_t w = 0; w < width; ++w) {
                    const uint32_t index = w + h * width;
                    unsigned char alpha = glyph->bitmap.buffer[index];
                    data[4 * index] = 0xff;       // red
                    data[4 * index + 1] = 0xff;   // green
                    data[4 * index + 2] = 0xff;   // blue
                    data[4 * index + 3] = alpha;  // alpha
                }
            }
            // Set subtexture (of glyph) data with this expanded one
            m_texture->SetData(data, {x, y}, width, height);
            // We can now delete this
            delete[] data;
        }

        float uvOffsetX = static_cast<float>(x) / static_cast<float>(m_width);
        float uvOffsetY = static_cast<float>(y) / static_cast<float>(m_height);

        FontCharacter character = {static_cast<float>(glyph->advance.x >> 6),
                                   static_cast<float>(glyph->advance.y >> 6),
                                   static_cast<float>(glyph->bitmap.width),
                                   static_cast<float>(glyph->bitmap.rows),
                                   static_cast<float>(glyph->bitmap_left),
                                   static_cast<float>(glyph->bitmap_top),
                                   uvOffsetX,
                                   uvOffsetY,
                                   uvOffsetX + static_cast<float>(glyph->bitmap.width) / static_cast<float>(m_width),
                                   uvOffsetY + static_cast<float>(glyph->bitmap.rows) / static_cast<float>(m_height)};

        m_characters.emplace(i, character);

        rowh = std::fmax(rowh, glyph->bitmap.rows);
        x += glyph->bitmap.width + 1;
    }

    LOG_ENGINE_TRACE("Generated a {} x {} ({} kb) texture atlas for font: {} - {}", m_width, m_height, m_width * m_height / 1024, face->family_name,
                     face->style_name);
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
}  // namespace de
