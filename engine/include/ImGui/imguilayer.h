#pragma once

#include "Core/layer.h"

namespace de {
class ImGuiLayer : public Layer {
public:
    ImGuiLayer();

    ~ImGuiLayer() override;

    /** Called once, when the layer is added to stack. */
    void OnAttach() override;

    /** Called once when the layer is removed from stack. */
    void OnDetach() override;

    /** Initializes a new ImGui frame. */
    void Begin();

    /** Handles rendering of ImGui. Call this after defining what to render in OnImGuiUpdate. */
    void End();

private:
    float time = 0.0f;
};
}  // namespace de
