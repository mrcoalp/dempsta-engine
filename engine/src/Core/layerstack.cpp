#include "Core/layerstack.h"

namespace de {
    LayerStack::~LayerStack() {
        for (auto& layer : layers) {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushOverlay(Layer* overlay) {
        layers.emplace_back(overlay);
    }

    void LayerStack::PopOverlay(Layer* overlay) {
        auto it = std::find(layers.begin() + lastInsertedLayerIndex, layers.end(), overlay);
        if (it != layers.end()) {
            overlay->OnDetach();
            layers.erase(it);
        }
    }

    void LayerStack::PushLayer(Layer* layer) {
        layers.emplace(layers.begin() + lastInsertedLayerIndex, layer);
        ++lastInsertedLayerIndex;
    }

    void LayerStack::PopLayer(Layer* layer) {
        auto it = std::find(layers.begin(), layers.begin() + lastInsertedLayerIndex, layer);
        if (it != layers.begin() + lastInsertedLayerIndex) {
            layer->OnDetach();
            layers.erase(it);
            --lastInsertedLayerIndex;
        }
    }
}
