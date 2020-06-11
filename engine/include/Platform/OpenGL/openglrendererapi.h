#pragma once

#include "Core/core.h"
#include "Renderer/rendererapi.h"

namespace de {
class OpenGLRendererAPI : public RendererAPI {
public:
    OpenGLRendererAPI() = default;

    void Init() override;

    void SetViewport(unsigned x, unsigned y, unsigned width, unsigned height) override;

    void Clear(glm::vec4& color) override;

    void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
};
}  // namespace de
