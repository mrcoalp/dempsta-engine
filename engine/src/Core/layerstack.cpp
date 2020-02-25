#include "Core/layerstack.h"

namespace de {
    LayerStack::~LayerStack() {
        for (auto& _layer : layers) {
            _layer->OnDetach();
            delete _layer;
        }
    }

    void LayerStack::PushOverlay(Layer* overlay) {
        layers.emplace_back(overlay);
    }

    void LayerStack::PopOverlay(Layer* overlay) {
        auto _it = std::find(layers.begin() + lastInsertedLayerIndex, layers.end(), overlay);
        if (_it != layers.end()) {
            overlay->OnDetach();
            layers.erase(_it);
        }
    }

    void LayerStack::PushLayer(Layer* layer) {
        layers.emplace(layers.begin() + lastInsertedLayerIndex, layer);
        ++lastInsertedLayerIndex;
    }

    void LayerStack::PopLayer(Layer* layer) {
        auto _it = std::find(layers.begin(), layers.begin() + lastInsertedLayerIndex, layer);
        if (_it != layers.begin() + lastInsertedLayerIndex) {
            layer->OnDetach();
            layers.erase(_it);
            --lastInsertedLayerIndex;
        }
    }
}
