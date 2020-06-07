#pragma once

#include <glad/glad.h>

#include "Core/pch.h"
#include "Renderer/texture.h"

namespace de {
class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    explicit OpenGLTexture2D(const std::string& filePath);

    ~OpenGLTexture2D();

    [[nodiscard]] unsigned GetWidth() const override;

    [[nodiscard]] unsigned GetHeight() const override;

    void SetData(void* data, uint32_t size) override;

    void Bind(unsigned slot) const override;

private:
    std::string m_filePath;
    unsigned m_width, m_height;
    unsigned m_rendererId;
    GLenum m_dataFormat;
};
}  // namespace de
