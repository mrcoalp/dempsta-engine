#pragma once

#include "Core/core.h"
#include "Scene/entity.h"

namespace de {
class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;

    explicit SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

private:
    Ref<Scene> m_context;
    Entity m_selected;

    void drawEntityNode(Entity entity);
    void drawTransformNode(Entity entity);
    void drawScriptingNode(Entity entity);
};
}  // namespace de
