#include "Renderer/label.h"

#include <glad/glad.h>

namespace de {
Label::Label(const char* fontPath, float size) {
    m_Atlas = ftgl::texture_atlas_new(512, 512, 2);
    m_Font = ftgl::texture_font_new_from_file(m_Atlas, size, fontPath);
    m_texture = Texture2D::Create(512, 512);
    m_texture->SetData(m_Atlas->data, 512 * 512 * 4);
}

ftgl::texture_glyph_t* Label::GetGylph(const char* codePoint) const {
    return ftgl::texture_font_get_glyph(m_Font, codePoint);
}
}  // namespace de
