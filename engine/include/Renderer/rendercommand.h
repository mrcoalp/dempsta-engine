#pragma once

#include "Core/core.h"
#include "Renderer/rendererapi.h"

namespace de {
class RenderCommand {
public:
    inline static void Clear(glm::vec4 color) { s_rendererApi->Clear(color); };

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
        s_rendererApi->DrawIndexed(vertexArray);
    }

private:
    static RendererAPI* s_rendererApi;
};
}  // namespace de
