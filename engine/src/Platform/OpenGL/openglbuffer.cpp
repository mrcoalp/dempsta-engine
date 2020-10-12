#include "Platform/OpenGL/openglbuffer.h"

#include <glad/glad.h>

#include "Platform/OpenGL/openglutils.h"

namespace de {
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    GL_CALL(glCreateBuffers, 1, &m_rendererId);
    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_rendererId);
    GL_CALL(glBufferData, GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
    GL_CALL(glCreateBuffers, 1, &m_rendererId);
    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_rendererId);
    GL_CALL(glBufferData, GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { GL_CALL(glDeleteBuffers, 1, &m_rendererId); }

void OpenGLVertexBuffer::Bind() const { GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_rendererId); }

void OpenGLVertexBuffer::UnBind() const { GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
    GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_rendererId);
    GL_CALL(glBufferSubData, GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_count(count) {
    GL_CALL(glCreateBuffers, 1, &m_rendererId);
    GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    GL_CALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { GL_CALL(glDeleteBuffers, 1, &m_rendererId); }

void OpenGLIndexBuffer::Bind() const { GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, m_rendererId); }

void OpenGLIndexBuffer::UnBind() const { GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0); }
}  // namespace de
