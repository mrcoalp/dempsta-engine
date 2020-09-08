#include "Renderer/text.h"

#include <glad/glad.h>

namespace de {
Text::Text(const char* fontPath, float size) {
    m_Atlas.reset(ftgl::texture_atlas_new(512, 512, 2));
    m_Font.reset(ftgl::texture_font_new_from_file(m_Atlas.get(), size, fontPath));
    m_texture = Texture2D::Create(1, 1);
    uint32_t _white = 0xffffffff;
    m_texture->SetData(&_white, sizeof(uint32_t));
}

ftgl::texture_glyph_t* Text::GetGylph(const char* codePoint) const {
    return ftgl::texture_font_get_glyph(m_Font.get(), codePoint);
}

void Text::Bind(unsigned slot) const { glBindTextureUnit(slot, m_Atlas->id); }
}  // namespace de
