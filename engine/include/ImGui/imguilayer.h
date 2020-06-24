#pragma once

#include "Core/layer.h"

namespace de {
class ImGuiLayer : public Layer {
public:
    ImGuiLayer();

    ~ImGuiLayer() override;

    void OnAttach() override;

    void OnDetach() override;

    void Begin();

    void End();

private:
    float time = 0.0f;
};
}  // namespace de
