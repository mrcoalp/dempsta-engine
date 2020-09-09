#pragma once

#include <freetype-gl.h>

#include "Core/core.h"
#include "Renderer/texture.h"

namespace de {
class Label {
public:
    Label(const char* fontPath, float size);

    [[nodiscard]] inline unsigned GetID() const { return m_Atlas->id; }

    [[nodiscard]] inline const std::string& GetContent() const { return m_content; }

    [[nodiscard]] ftgl::texture_glyph_t* GetGylph(const char* codePoint) const;

    [[nodiscard]] const Ref<Texture2D>& GetTexture() const { return m_texture; }

    void SetContent(const std::string& content) { m_content = content; }

private:
    ftgl::texture_atlas_t* m_Atlas{nullptr};
    ftgl::texture_font_t* m_Font{nullptr};
    std::string m_content{""};
    Ref<Texture2D> m_texture{nullptr};
};
}  // namespace de
