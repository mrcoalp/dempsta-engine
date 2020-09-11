#pragma once

#include <freetype-gl.h>

#include "Core/core.h"
#include "Renderer/texture.h"

namespace de {
class Font {
public:
    Font(const std::string& name, const std::string& source, float size);

    [[nodiscard]] const Ref<Texture2D>& GetTexture() const { return m_texture; }

    [[nodiscard]] ftgl::texture_glyph_t* GetGylph(const char* codePoint) const;

    [[nodiscard]] float GetKerning(ftgl::texture_glyph_t* glyph, const char* codePoint) const;

    ftgl::texture_atlas_t* m_atlas{nullptr};
private:
    ftgl::texture_font_t* m_font{nullptr};
    Ref<Texture2D> m_texture{nullptr};
};
}  // namespace de
