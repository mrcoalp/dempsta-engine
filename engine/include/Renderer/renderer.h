#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"
#include "Renderer/camera.h"
#include "Renderer/rendercommand.h"
#include "Renderer/shader.h"

namespace de {
class Renderer {
public:
    static void OnWindowResize(unsigned width, unsigned height);

    static void BeginScene(OrthographicCamera& camera);

    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
                       const glm::mat4& transform = glm::mat4(1.0f));

    inline static RendererAPI::API GetApi() { return RendererAPI::GetApi(); }

private:
    struct SceneData {
        glm::mat4 projectionViewMatrix;
    };

    static SceneData* s_sceneData;
};
}  // namespace de
