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

        // TODO(MPINTO): Remove after proper implementation
        void OnImGuiRender() override;

    private:
        float time = 0.0f;
    };
}
