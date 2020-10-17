#include "Core/layerstack.h"

namespace de {
LayerStack::~LayerStack() {
    for (auto& _layer : m_layers) {
        _layer->OnDetach();
        delete _layer;
    }
}

void LayerStack::PushOverlay(Layer* overlay) { m_layers.emplace_back(overlay); }

void LayerStack::PopOverlay(Layer* overlay) {
    auto _it = std::find(m_layers.begin() + m_lastInsertedLayerIndex, m_layers.end(), overlay);
    if (_it != m_layers.end()) {
        overlay->OnDetach();
        m_layers.erase(_it);
    }
}

void LayerStack::PushLayer(Layer* layer) {
    m_layers.emplace(m_layers.begin() + m_lastInsertedLayerIndex, layer);
    ++m_lastInsertedLayerIndex;
}

void LayerStack::PopLayer(Layer* layer) {
    auto _it = std::find(m_layers.begin(), m_layers.begin() + m_lastInsertedLayerIndex, layer);
    if (_it != m_layers.begin() + m_lastInsertedLayerIndex) {
        layer->OnDetach();
        m_layers.erase(_it);
        --m_lastInsertedLayerIndex;
    }
}
}  // namespace de
