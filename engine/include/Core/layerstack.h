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

#define STACK_IT [[nodiscard]] std::vector<Layer*>::iterator
#define STACK_IT_CONST [[nodiscard]] std::vector<Layer*>::const_iterator
#define STACK_IT_REV [[nodiscard]] std::vector<Layer*>::reverse_iterator
#define STACK_IT_CONST_REV [[nodiscard]] std::vector<Layer*>::const_reverse_iterator

    STACK_IT begin() { return layers.begin(); }

    STACK_IT end() { return layers.end(); }

    STACK_IT_REV rbegin() { return layers.rbegin(); }

    STACK_IT_REV rend() { return layers.rend(); }

    STACK_IT_CONST begin() const { return layers.begin(); }

    STACK_IT_CONST end() const { return layers.end(); }

    STACK_IT_CONST_REV rbegin() const { return layers.rbegin(); }

    STACK_IT_CONST_REV rend() const { return layers.rend(); }

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
