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

    [[nodiscard]] inline uint32_t GetWidth() const override { return m_width; }

    [[nodiscard]] inline uint32_t GetHeight() const override { return m_height; }

    void SetData(void* data, uint32_t size) override;

    void Bind(unsigned slot) const override;

    [[nodiscard]] inline bool Equals(const Texture& other) const override {
        return ((OpenGLTexture2D&)other).GetRendererID() == m_rendererId;
    }

    [[nodiscard]] inline uint32_t GetRendererID() const override { return m_rendererId; }

private:
    std::string m_filePath;
    uint32_t m_width, m_height;
    uint32_t m_rendererId;
    GLenum m_dataFormat;
};
}  // namespace de
