#pragma once

#include "Renderer/vertexarray.h"

namespace de {
class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();

    ~OpenGLVertexArray();

    void Bind() const override;

    void UnBind() const override;

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;

    void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    [[nodiscard]] inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
        return m_vertexBuffers;
    }

    [[nodiscard]] inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; };

private:
    uint32_t m_rendererId;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
};
}  // namespace de
