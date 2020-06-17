#pragma once

#include "Renderer/framebuffer.h"

namespace de {
class OpenGLFrameBuffer : public FrameBuffer {
public:
    explicit OpenGLFrameBuffer(const FrameBufferConfig& config);
    ~OpenGLFrameBuffer();

    void Bind() override;

    void Unbind() override;

    [[nodiscard]] inline const FrameBufferConfig& GetConfig() const override { return m_config; }

    [[nodiscard]] inline uint32_t GetColorAttachment() const override { return m_colorAttachment; }

    void Resize(uint32_t width, uint32_t height) override;

    void Create();

    void Destroy();

private:
    FrameBufferConfig m_config;
    uint32_t m_rendererID = 0;
    uint32_t m_colorAttachment, m_depthAttachment;
};
}  // namespace de
