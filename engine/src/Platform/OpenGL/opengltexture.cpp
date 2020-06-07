#include "Platform/OpenGL/opengltexture.h"

#include <stb_image.h>

#include "Core/core.h"

namespace de {
OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_dataFormat(0) {
    GLenum _internalFormat = GL_RGBA8;
    m_dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1, _internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) : m_filePath(filePath), m_dataFormat(0) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    auto data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    DE_ASSERT(data != nullptr, "Couldn't load texture: {0}! Reason: {1}", filePath, stbi_failure_reason())
    m_width = width;
    m_height = height;

    GLenum _internalFormat = 0;
    GLenum _dataFormat = 0;

    if (channels == 3) {
        _internalFormat = GL_RGB8;
        m_dataFormat = _dataFormat = GL_RGB;
    } else if (channels == 4) {
        _internalFormat = GL_RGBA8;
        m_dataFormat = _dataFormat = GL_RGBA;
    }

    DE_ASSERT(_internalFormat && _dataFormat, "Invalid RGB formats!")

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1, _internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, _dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_rendererId); }

unsigned OpenGLTexture2D::GetWidth() const { return m_width; }

unsigned OpenGLTexture2D::GetHeight() const { return m_height; }

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    uint32_t _bytesPerPixel = m_dataFormat == GL_RGB ? 3 : 4;
    DE_ASSERT(size == m_width * m_height * _bytesPerPixel, "Size must be entire texture!")
    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(unsigned slot) const { glBindTextureUnit(slot, m_rendererId); }
}  // namespace de
