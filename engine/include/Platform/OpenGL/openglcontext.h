#pragma once

#include "Renderer/context.h"

struct GLFWwindow;

namespace de {
class OpenGLContext : public Context {
public:
    explicit OpenGLContext(GLFWwindow* windowHandle);

    /** Initializes glfw context and loads GL loader. */
    void Init() final;

    /** Swaps chain buffers. */
    void SwapBuffers() final;

private:
    GLFWwindow* windowHandle;
};
}  // namespace de
