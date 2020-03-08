#pragma once

#include "Renderer/context.h"

struct GLFWwindow;

namespace de {
    class OpenGLContext : public Context {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);

        void Init() final;

        void SwapBuffers() final;

    private:
        GLFWwindow* windowHandle;
    };
}
