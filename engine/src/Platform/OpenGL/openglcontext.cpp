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

        LOG_ENGINE_INFO("OpenGL Renderer:\n"
                        "\tVendor - {0}\n"
                        "\tRenderer - {1}\n"
                        "\tVersion - {2}",
                        glGetString(GL_VENDOR),
                        glGetString(GL_RENDERER),
                        glGetString(GL_VERSION)
        );
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(windowHandle);
    }
}
