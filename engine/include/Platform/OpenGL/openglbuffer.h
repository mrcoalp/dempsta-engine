#pragma once

#include "Renderer/buffer.h"

namespace de {
class OpenGLVertexBuffer : public VertexBuffer {
public:
    explicit OpenGLVertexBuffer(uint32_t size);

    OpenGLVertexBuffer(float* vertices, uint32_t size);

    ~OpenGLVertexBuffer() override;

    /** Binds vertex buffer. */
    void Bind() const override;

    /** Unbinds vertex buffer. */
    void UnBind() const override;

    /** Sets the buffer layout.
     * @param layout Buffer layout
     */
    void SetLayout(const BufferLayout& layout) override { m_layout = layout; };

    /** Getter of buffer layout. */
    [[nodiscard]] inline const BufferLayout& GetLayout() const override { return m_layout; }

    /** Sets the buffer data.
     * @param data Data to set buffer
     * @param size Size of data
     */
    void SetData(const void* data, uint32_t size) override;

private:
    uint32_t m_rendererId;
    BufferLayout m_layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

    ~OpenGLIndexBuffer() override;

    /** Binds the index buffer. */
    void Bind() const override;

    /** Unbinds the index buffer. */
    void UnBind() const override;

    /** Getter for the index count. */
    [[nodiscard]] inline uint32_t GetCount() const override { return m_count; }

private:
    uint32_t m_rendererId;
    uint32_t m_count;
};
}  // namespace de
