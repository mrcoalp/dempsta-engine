#pragma once

#include "Core/layer.h"

namespace de {
class LayerStack {
public:
    LayerStack() = default;

    ~LayerStack();

    /**
     * @brief Push a new overlay.
     * Overlays stay on top of the stack, followed by normal layers.
     * @param overlay Layer to be inserted in stack.
     */
    void PushOverlay(Layer* overlay);

    /**
     * @brief Pop overlay. Trigger OnDetach.
     * Overlays stay on top of the stack, followed by normal layers.
     * @param overlay Layer to be popped from stack.
     */
    void PopOverlay(Layer* overlay);

    /**
     * @brief Push a new layer to stack.
     * @param layer Layer to be inserted in stack.
     */
    void PushLayer(Layer* layer);

    /**
     * @brief Pop layer from stack. Trigger OnDetach.
     * @param layer Layer to be popped.
     */
    void PopLayer(Layer* layer);

    [[nodiscard]] std::vector<Layer*>::iterator begin() { return layers.begin(); }
    [[nodiscard]] std::vector<Layer*>::iterator end() { return layers.end(); }

    [[nodiscard]] std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
    [[nodiscard]] std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }

    [[nodiscard]] std::vector<Layer*>::const_iterator begin() const { return layers.begin(); }
    [[nodiscard]] std::vector<Layer*>::const_iterator end() const { return layers.end(); }

    [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
    [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rend() const { return layers.rend(); }

private:
    /**
     * @brief Vector containing all layers and overlays.
     */
    std::vector<Layer*> layers;
    /**
     * @brief Index to keep track of layer/overlay separation.
     */
    unsigned int lastInsertedLayerIndex = 0;
};
}  // namespace de
