#pragma once

#include "Core/core.h"
#include "Renderer/vertexarray.h"

namespace de {
class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();

    ~OpenGLVertexArray();

    void Bind() const override;

    void UnBind() const override;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;

    void AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {
        return m_vertexBuffers;
    }

    [[nodiscard]] inline const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; };

private:
    uint32_t m_rendererId;
    std::vector<Ref<VertexBuffer>> m_vertexBuffers;
    Ref<IndexBuffer> m_indexBuffer;
};
}  // namespace de
