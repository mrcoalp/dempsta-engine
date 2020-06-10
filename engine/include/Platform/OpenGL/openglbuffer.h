#pragma once

#include "Renderer/buffer.h"

namespace de {
class OpenGLVertexBuffer : public VertexBuffer {
public:
    explicit OpenGLVertexBuffer(uint32_t size);

    OpenGLVertexBuffer(float* vertices, uint32_t size);

    ~OpenGLVertexBuffer() override;

    void Bind() const override;

    void UnBind() const override;

    void SetLayout(const BufferLayout& layout) override { m_layout = layout; };

    [[nodiscard]] inline const BufferLayout& GetLayout() const override { return m_layout; }

    void SetData(const void* data, uint32_t size) override;

private:
    uint32_t m_rendererId;
    BufferLayout m_layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

    ~OpenGLIndexBuffer() override;

    void Bind() const override;

    void UnBind() const override;

    [[nodiscard]] inline uint32_t GetCount() const override { return m_count; }

private:
    uint32_t m_rendererId;
    uint32_t m_count;
};
}  // namespace de
