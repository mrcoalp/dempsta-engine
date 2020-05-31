#include "Platform/OpenGL/openglcontext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/core.h"

namespace de {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : windowHandle(windowHandle) {
    DE_ASSERT(windowHandle != nullptr, "OpenGLContext not initialized!")
}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(windowHandle);
    // Initialize GLAD
    int _statusGLAD = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    DE_ASSERT(_statusGLAD, "GLAD not initialized!")
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(windowHandle); }
}  // namespace de
