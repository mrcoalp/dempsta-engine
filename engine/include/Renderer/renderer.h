#pragma once

#include <glm/glm.hpp>

#include "Renderer/camera.h"
#include "Renderer/rendercommand.h"
#include "Renderer/shader.h"

namespace de {
class Renderer {
public:
    static void BeginScene(OrthographicCamera& camera);

    static void EndScene();

    static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    inline static RendererAPI::API GetApi() { return RendererAPI::GetApi(); }

private:
    struct SceneData {
        glm::mat4 projectionViewMatrix;
    };

    static SceneData* s_sceneData;
};
}  // namespace de
