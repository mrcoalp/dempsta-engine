#pragma once

#include "Renderer/rendererapi.h"

namespace de {
class OpenGLRendererAPI : public RendererAPI {
public:
    OpenGLRendererAPI() = default;

    void Clear(glm::vec4& color) override;

    void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};
}  // namespace de
