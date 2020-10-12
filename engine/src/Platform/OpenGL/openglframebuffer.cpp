#include "Platform/OpenGL/openglframebuffer.h"

#include <glad/glad.h>

#include "Platform/OpenGL/openglutils.h"

namespace de {
OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferConfig& config) : m_config(config) { Create(); }

OpenGLFrameBuffer::~OpenGLFrameBuffer() { Destroy(); }

void OpenGLFrameBuffer::Create() {
    if (m_rendererID != 0u) {
        Destroy();
    }

    GL_CALL(glCreateFramebuffers, 1, &m_rendererID);
    GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_rendererID);

    GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &m_colorAttachment);
    GL_CALL(glBindTexture, GL_TEXTURE_2D, m_colorAttachment);
    GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA8, m_config.width, m_config.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

    GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &m_depthAttachment);
    GL_CALL(glBindTexture, GL_TEXTURE_2D, m_depthAttachment);
    GL_CALL(glTexStorage2D, GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_config.width, m_config.height);
    GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

    uint32_t status = GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER);
    DE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete! Error: {0:#X}", status);

    GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Destroy() {
    GL_CALL(glDeleteFramebuffers, 1, &m_rendererID);
    GL_CALL(glDeleteTextures, 1, &m_colorAttachment);
    GL_CALL(glDeleteTextures, 1, &m_depthAttachment);
}

void OpenGLFrameBuffer::Bind() {
    GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_rendererID);
    GL_CALL(glViewport, 0, 0, m_config.width, m_config.height);
}

void OpenGLFrameBuffer::Unbind() { GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0); }

void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
    m_config.width = width;
    m_config.height = height;
    Create();
}
}  // namespace de
