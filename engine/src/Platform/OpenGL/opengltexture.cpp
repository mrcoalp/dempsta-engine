#include "Platform/OpenGL/opengltexture.h"

#include <stb_image.h>

#include "Core/core.h"

namespace de {
OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_dataFormat(0) {
    GLenum internalFormat = GL_RGBA8;
    m_dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1, internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) : m_filePath(filePath), m_dataFormat(0) {
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(1);
    auto* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    DE_ASSERT(data != nullptr, "Couldn't load texture: {0}! Reason: {1}", filePath, stbi_failure_reason())
    m_width = width;
    m_height = height;

    GLenum internalFormat = 0;
    GLenum dataFormat = 0;

    if (channels == 3) {
        internalFormat = GL_RGB8;
        m_dataFormat = dataFormat = GL_RGB;
    } else if (channels == 4) {
        internalFormat = GL_RGBA8;
        m_dataFormat = dataFormat = GL_RGBA;
        glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    DE_ASSERT(internalFormat && dataFormat, "Invalid RGB formats!")

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1, internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_rendererId); }

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    uint32_t bytesPerPixel = m_dataFormat == GL_RGB ? 3 : 4;
    DE_ASSERT(size == m_width * m_height * bytesPerPixel, "Size must be entire texture!")
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(unsigned slot) const { glBindTextureUnit(slot, m_rendererId); }
}  // namespace de
