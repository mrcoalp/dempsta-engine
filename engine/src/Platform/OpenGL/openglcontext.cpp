#include "Platform/OpenGL/openglcontext.h"

#include <glad/glad.h>
// GLFW must come after
#include <GLFW/glfw3.h>

#include "Core/core.h"

namespace de {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle) {
    DE_ASSERT(windowHandle != nullptr, "OpenGLContext not initialized!")
}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(m_windowHandle);
    // Initialize GLAD
    int _statusGLAD = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    DE_ASSERT(_statusGLAD, "GLAD not initialized!")
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_windowHandle); }
}  // namespace de
