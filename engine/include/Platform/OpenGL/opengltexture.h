#pragma once

#include "Core/pch.h"
#include "Renderer/texture.h"

namespace de {
class OpenGLTexture2D : public Texture2D {
public:
    explicit OpenGLTexture2D(const std::string& filePath);

    ~OpenGLTexture2D();

    [[nodiscard]] unsigned GetWidth() const override;

    [[nodiscard]] unsigned GetHeight() const override;

    void Bind(unsigned slot) const override;

private:
    std::string m_filePath;
    unsigned m_width, m_height;
    unsigned m_rendererId;
};
}  // namespace de
