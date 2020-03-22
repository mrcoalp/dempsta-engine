#pragma once

#include "Renderer/buffer.h"

namespace de {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);

        ~OpenGLVertexBuffer() override;

        void Bind() const override;

        void UnBind() const override;

    private:
        uint32_t rendererId;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

        ~OpenGLIndexBuffer() override;

        void Bind() const override;

        void UnBind() const override;

        [[nodiscard]] inline uint32_t GetCount() const override { return count; }

    private:
        uint32_t rendererId;
        uint32_t count;
    };
}
