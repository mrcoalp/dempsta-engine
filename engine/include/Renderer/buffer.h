#pragma once

#include "Core/pch.h"

namespace de {
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;;

        virtual void Bind() const = 0;

        virtual void UnBind() const = 0;

        static VertexBuffer* Create(float* vertices, uint32_t size);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;;

        virtual void Bind() const = 0;

        virtual void UnBind() const = 0;

        [[nodiscard]] virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t* indices, uint32_t count);
    };
}
