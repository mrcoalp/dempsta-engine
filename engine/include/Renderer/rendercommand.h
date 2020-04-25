#pragma once

#include "Core/core.h"
#include "Renderer/rendererapi.h"

namespace de {
class RenderCommand {
public:
    inline static void Init() { s_rendererApi->Init(); }

    inline static void SetViewport(unsigned x, unsigned y, unsigned width, unsigned height) {
        s_rendererApi->SetViewport(x, y, width, height);
    }

    inline static void Clear(glm::vec4 color) { s_rendererApi->Clear(color); };

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_rendererApi->DrawIndexed(vertexArray); }

private:
    static Ref<RendererAPI> s_rendererApi;
};
}  // namespace de
