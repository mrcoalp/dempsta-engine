#pragma once

#include "Core/core.h"
#include "Scene/entity.h"

namespace de {
class ScenePanel {
public:
    ScenePanel() = default;

    explicit ScenePanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void UnSelectEntity();

    void OnImGuiRender();

private:
    Ref<Scene> m_context;
    Entity m_selected;

    void drawEntityNode(Entity entity);
};
}  // namespace de
