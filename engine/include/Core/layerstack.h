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

    [[nodiscard]] std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
    [[nodiscard]] std::vector<Layer*>::iterator end() { return m_layers.end(); }

    [[nodiscard]] std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
    [[nodiscard]] std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

    [[nodiscard]] std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
    [[nodiscard]] std::vector<Layer*>::const_iterator end() const { return m_layers.end(); }

    [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
    [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }

private:
    /**
     * @brief Vector containing all layers and overlays.
     */
    std::vector<Layer*> m_layers;
    /**
     * @brief Index to keep track of layer/overlay separation.
     */
    unsigned int m_lastInsertedLayerIndex = 0;
};
}  // namespace de
