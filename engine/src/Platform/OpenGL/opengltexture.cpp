#include "Platform/OpenGL/opengltexture.h"

#include <stb_image.h>

#include "Core/core.h"

namespace de {
OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_dataFormat(0) {
    // TODO(mpinto): Change this. Just for freetype render test
    GLenum internalFormat = GL_R8;
    m_dataFormat = GL_RED;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTextureStorage2D(m_rendererId, 1, internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) : m_filePath(filePath), m_dataFormat(0) {
    int width;
    int height;
    int channels;

    // stbi_set_flip_vertically_on_load(1);
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

void OpenGLTexture2D::SetData(void* data, const glm::vec2& offset) {
    glTextureSubImage2D(m_rendererId, 0, offset.x, offset.y, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::SetData(void* data, const glm::vec2& offset, unsigned width, unsigned height) {
    glTextureSubImage2D(m_rendererId, 0, offset.x, offset.y, width, height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(unsigned slot) const { glBindTextureUnit(slot, m_rendererId); }
}  // namespace de
