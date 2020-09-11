#include "Renderer/font.h"

namespace de {
Font::Font(const std::string& name, const std::string& source, float size) {
    m_atlas = ftgl::texture_atlas_new(512, 512, 1);
    m_font = ftgl::texture_font_new_from_file(m_atlas, size, source.c_str());
    DE_ASSERT(m_font != nullptr, "Failed to load font: {}", source)
    m_texture = Texture2D::Create(512, 512);
    m_texture->SetData(m_atlas->data, 512 * 512 * 4);
}

ftgl::texture_glyph_t* Font::GetGylph(const char* codePoint) const {
    return ftgl::texture_font_get_glyph(m_font, codePoint);
}

float Font::GetKerning(ftgl::texture_glyph_t* glyph, const char* codePoint) const {
    return texture_glyph_get_kerning(glyph, codePoint);
}
}  // namespace de
