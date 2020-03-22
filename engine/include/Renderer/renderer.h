#pragma once

namespace de {
    enum class RendererAPI {
        NONE    = 0,
        OPENGL  = 1
    };

    class Renderer {
    public:
        inline static RendererAPI GetApi() { return rendererApi; }

    private:
        static RendererAPI rendererApi;
    };
}
