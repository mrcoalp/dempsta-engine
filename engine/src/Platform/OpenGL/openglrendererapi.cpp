#include "Platform/OpenGL/openglrendererapi.h"

#include <glad/glad.h>

#include "Platform/OpenGL/openglutils.h"

namespace de {
static void GLAPIENTRY GLErrorCallback(GLenum, GLenum type, GLuint, GLenum, GLsizei, const GLchar* message, const void*) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        LOG_ENGINE_ERROR("** GL ERROR ** - {}", message);
    }
}

void OpenGLRendererAPI::Init() {
#ifdef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLErrorCallback, nullptr);
#endif
    GL_CALL(glEnable, GL_BLEND);
    GL_CALL(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CALL(glEnable, GL_DEPTH_TEST);
    GLint maxTexture;
    GL_CALL(glGetIntegerv, GL_MAX_TEXTURE_SIZE, &maxTexture);
    LOG_ENGINE_INFO(
        "OpenGL Renderer Initialized:\n"
        "\tVendor       - {0}\n"
        "\tRenderer     - {1}\n"
        "\tVersion      - {2}\n"
        "\tMax Texture  - {3}x{3}",
        GL_CALL(glGetString, GL_VENDOR), GL_CALL(glGetString, GL_RENDERER), GL_CALL(glGetString, GL_VERSION), maxTexture);
}

void OpenGLRendererAPI::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    GL_CALL(glViewport, x, y, width, height);
}

void OpenGLRendererAPI::Clear(glm::vec4& color) {
    GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
    GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
    uint32_t count = indexCount != 0u ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
    GL_CALL(glDrawElements, GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

uint32_t OpenGLRendererAPI::GetMaxTextureSize() const {
    GLint maxTextureSize;
    GL_CALL(glGetIntegerv, GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    return static_cast<uint32_t>(maxTextureSize);
}
}  // namespace de
