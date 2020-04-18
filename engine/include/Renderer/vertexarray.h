#pragma once

#include "Renderer/buffer.h"

namespace de {
class VertexArray {
public:
    ~VertexArray() = default;

    virtual void Bind() const = 0;

    virtual void UnBind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;

    virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

    [[nodiscard]] virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;

    [[nodiscard]] virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

    static VertexArray* Create();
};
}  // namespace de
