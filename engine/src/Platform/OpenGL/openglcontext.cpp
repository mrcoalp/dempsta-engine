#include "Platform/OpenGL/openglcontext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/core.h"

namespace de {
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : windowHandle(windowHandle) {
        DE_ASSERT((int) (windowHandle != nullptr), "OpenGLContext")
    }

    void OpenGLContext::Init() {
        glfwMakeContextCurrent(windowHandle);
        // Initialize GLAD
        int _statusGLAD = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        DE_ASSERT(_statusGLAD, "GLAD")
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(windowHandle);
    }
}
