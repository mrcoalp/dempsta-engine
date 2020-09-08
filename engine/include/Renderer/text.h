#pragma once

#include <freetype-gl.h>

namespace de {
class Text {
public:
    Text(const char* fontPath, float size) : m_fontPath(fontPath), m_size(size) {
        m_Atlas = ftgl::texture_atlas_new(512, 512, 1);
        m_Font = ftgl::texture_font_new_from_file(m_Atlas, size, fontPath);
    }

private:
    ftgl::texture_atlas_t* m_Atlas;
    ftgl::texture_font_t* m_Font;

    const char* m_fontPath;
    float m_size;
};
}
