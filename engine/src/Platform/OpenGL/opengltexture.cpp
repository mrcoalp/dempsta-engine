#include "Platform/OpenGL/opengltexture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Core/core.h"

namespace de {
OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) : m_filePath(filePath) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    auto data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    DE_ASSERT(data != nullptr, "Couldn't load texture: {0}! Reason: {1}", filePath, stbi_failure_reason())
    m_width = width;
    m_height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1, GL_RGB8, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE,
                        data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_rendererId); }

unsigned OpenGLTexture2D::GetWidth() const { return m_width; }

unsigned OpenGLTexture2D::GetHeight() const { return m_height; }

void OpenGLTexture2D::Bind(unsigned slot) const { glBindTextureUnit(slot, m_rendererId); }
}  // namespace de
