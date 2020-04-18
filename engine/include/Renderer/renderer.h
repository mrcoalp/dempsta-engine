#pragma once

namespace de {
enum class RendererAPI { None = 0, OpenGL = 1 };

class Renderer {
public:
    inline static RendererAPI GetApi() { return rendererApi; }

private:
    static RendererAPI rendererApi;
};
}  // namespace de
