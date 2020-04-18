#pragma once

#include <utility>

#include "Core/pch.h"

namespace de {
enum class ShaderDataType : uint8_t { None = 0, Vec, Vec2, Vec3, Vec4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::None:
            return 0;
        case ShaderDataType::Int:
        case ShaderDataType::Vec:
            return 4;
        case ShaderDataType::Int2:
        case ShaderDataType::Vec2:
            return 4 * 2;
        case ShaderDataType::Int3:
        case ShaderDataType::Vec3:
            return 4 * 3;
        case ShaderDataType::Int4:
        case ShaderDataType::Vec4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Bool:
            return 1;
    }
}

struct BufferElement {
    ShaderDataType type;
    std::string name;
    uint32_t size;
    uint32_t offset;
    bool normalized;

    BufferElement(ShaderDataType type, std::string name, bool normalized = false)
        : type(type), name(std::move(name)), size(ShaderDataTypeSize(type)), normalized(normalized), offset(0) {}

    [[nodiscard]] uint32_t GetComponentCount() const {
        switch (type) {
            case ShaderDataType::None:
                return 0;
            case ShaderDataType::Int:
            case ShaderDataType::Vec:
                return 1;
            case ShaderDataType::Int2:
            case ShaderDataType::Vec2:
                return 2;
            case ShaderDataType::Int3:
            case ShaderDataType::Vec3:
                return 3;
            case ShaderDataType::Int4:
            case ShaderDataType::Vec4:
                return 4;
            case ShaderDataType::Mat3:
                return 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4;
            case ShaderDataType::Bool:
                return 1;
        }
    }
};

class BufferLayout {
public:
    BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferElement> elements) : m_elements(elements) {
        calculateOffsetAndStride();
    }

    [[nodiscard]] inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

    [[nodiscard]] inline uint32_t GetStride() const { return m_stride; }

    // Handle auto iterator for loops
    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

private:
    void calculateOffsetAndStride() {
        m_stride = 0;
        for (auto& element : m_elements) {
            element.offset = m_stride;
            m_stride += element.size;
        }
    }

    std::vector<BufferElement> m_elements;
    uint32_t m_stride{0};
};

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;
    ;

    virtual void Bind() const = 0;

    virtual void UnBind() const = 0;

    virtual void SetLayout(const BufferLayout& layout) = 0;

    [[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;

    static VertexBuffer* Create(float* vertices, uint32_t size);
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
    ;

    virtual void Bind() const = 0;

    virtual void UnBind() const = 0;

    [[nodiscard]] virtual uint32_t GetCount() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t count);
};
}  // namespace de
