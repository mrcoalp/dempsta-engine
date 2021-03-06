#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"
#include "Renderer/vertexarray.h"

namespace de {
class RendererAPI {
public:
    virtual ~RendererAPI() = default;

    enum class API { None = 0, OpenGL = 1 };

    virtual void Init() = 0;

    virtual void SetViewport(unsigned x, unsigned y, unsigned width, unsigned height) = 0;

    virtual void Clear(glm::vec4& color) = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;

    [[nodiscard]] virtual uint32_t GetMaxTextureSize() const = 0;

    inline static API GetApi() { return s_api; }

private:
    static API s_api;
};
}  // namespace de
