#pragma once

#include <dempsta.h>

#include "Panels/scenepanel.h"

namespace de {
class DempstaEditor : public Layer {
public:
    explicit DempstaEditor();

    void OnAttach() final;

    void OnDetach() final;

    void OnUpdate(const TimeStep& ts) final;

    void OnEvent(Event& e) final;

    void OnImGuiRender() final;

private:
    Ref<FrameBuffer> m_frameBuffer;
    TimeStep m_ts;
    float m_timeAccumulator{0.f};
    float m_fps{0.f};
    Ref<Scene> m_activeScene;

    glm::vec2 m_viewportSize{glm::vec2(0.f)};

    ScenePanel m_sceneHierarchyPanel;
};
}  // namespace de
