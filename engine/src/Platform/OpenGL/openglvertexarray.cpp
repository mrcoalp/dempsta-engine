#include "Platform/OpenGL/openglvertexarray.h"

#include <glad/glad.h>

#include "Core/core.h"

namespace de {
static GLenum ShaderDataTypeToOpenGLType(const ShaderDataType& type) {
    switch (type) {
        case ShaderDataType::None:
            return -1;
        case ShaderDataType::Vec:
        case ShaderDataType::Vec2:
        case ShaderDataType::Vec3:
        case ShaderDataType::Vec4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
    }
}

OpenGLVertexArray::OpenGLVertexArray() { glCreateVertexArrays(1, &m_rendererId); }

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_rendererId); };

void OpenGLVertexArray::Bind() const { glBindVertexArray(m_rendererId); }

void OpenGLVertexArray::UnBind() const { glBindVertexArray(0); }

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
    auto _layout = vertexBuffer->GetLayout();

    DE_ASSERT(!_layout.GetElements().empty(), "Tried to add a vertex buffer with no layout elements!!")

    glBindVertexArray(m_rendererId);
    vertexBuffer->Bind();

    unsigned _i = 0;
    for (const auto& element : _layout) {
        glEnableVertexAttribArray(_i);
        glVertexAttribPointer(_i, element.GetComponentCount(), ShaderDataTypeToOpenGLType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE, _layout.GetStride(),
                              (void*)(uintptr_t)element.offset);
        ++_i;
    }

    m_vertexBuffers.emplace_back(vertexBuffer);
}

void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_rendererId);
    indexBuffer->Bind();
    m_indexBuffer = indexBuffer;
}
}  // namespace de
