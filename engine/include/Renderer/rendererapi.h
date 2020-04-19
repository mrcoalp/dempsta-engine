#pragma once

#include <glm/glm.hpp>

#include "Renderer/vertexarray.h"

namespace de {
class RendererAPI {
public:
    enum class API { None = 0, OpenGL = 1 };

    virtual void Clear(glm::vec4& color) = 0;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

    inline static API GetApi() { return s_api; }

private:
    static API s_api;
};
}  // namespace de
