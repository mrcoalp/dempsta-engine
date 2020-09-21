#pragma once

#include <glad/glad.h>

#include "Renderer/texture.h"

namespace de {
class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    explicit OpenGLTexture2D(const std::string& filePath);

    ~OpenGLTexture2D() override;

    [[nodiscard]] inline uint32_t GetWidth() const final { return m_width; }

    [[nodiscard]] inline uint32_t GetHeight() const final { return m_height; }

    void SetData(void* data, const glm::vec2& offset) final;

    void SetData(void* data, const glm::vec2& offset, unsigned width, unsigned height) final;

    void Bind(unsigned slot = 0) const final;

    [[nodiscard]] inline bool Equals(const Texture& other) const final { return ((OpenGLTexture2D&)other).GetRendererID() == m_rendererId; }

    [[nodiscard]] inline uint32_t GetRendererID() const final { return m_rendererId; }

private:
    std::string m_filePath;
    uint32_t m_width, m_height;
    uint32_t m_rendererId;
    GLenum m_dataFormat;
};
}  // namespace de
