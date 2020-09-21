#include "Platform/OpenGL/openglrendererapi.h"

#include <glad/glad.h>

namespace de {
void OpenGLRendererAPI::Init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    GLint maxTexture;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexture);
    LOG_ENGINE_INFO(
        "OpenGL Renderer Initialized:\n"
        "\tVendor       - {0}\n"
        "\tRenderer     - {1}\n"
        "\tVersion      - {2}\n"
        "\tMax Texture  - {3}x{3}",
        glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), maxTexture);
}

void OpenGLRendererAPI::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) { glViewport(x, y, width, height); }

void OpenGLRendererAPI::Clear(glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
    uint32_t count = indexCount != 0u ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

uint32_t OpenGLRendererAPI::GetMaxTextureSize() const {
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    return static_cast<uint32_t>(maxTextureSize);
}
}  // namespace de
