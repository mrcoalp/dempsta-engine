#include "Platform/OpenGL/openglframebuffer.h"

#include <glad/glad.h>

namespace de {
OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferConfig& config) : m_config(config) { Create(); }

OpenGLFrameBuffer::~OpenGLFrameBuffer() { Destroy(); }

void OpenGLFrameBuffer::Create() {
    if (m_rendererID != 0u) {
        Destroy();
    }

    glCreateFramebuffers(1, &m_rendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_config.width, m_config.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_config.width, m_config.height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

    uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    DE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete! Error: {0:#X}", status);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Destroy() {
    glDeleteFramebuffers(1, &m_rendererID);
    glDeleteTextures(1, &m_colorAttachment);
    glDeleteTextures(1, &m_depthAttachment);
}

void OpenGLFrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
    glViewport(0, 0, m_config.width, m_config.height);
}

void OpenGLFrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
    m_config.width = width;
    m_config.height = height;
    Create();
}
}  // namespace de
