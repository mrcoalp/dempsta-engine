#pragma once

#include "Core/pch.h"
#include "Core/timestep.h"
#include "Events/event.h"

namespace de {
class Layer {
public:
    explicit Layer(std::string name = "Layer") : debugName(std::move(name)) {}

    virtual ~Layer() = default;

    /** Called once when the layer is added to stack. */
    virtual void OnAttach(){};

    /** Called once when the layer is removed from stack. */
    virtual void OnDetach(){};

    /**
     * Called every frame.
     * @param ts Struct containing info about time passed from last frame.
     */
    virtual void OnUpdate(const TimeStep& ts){};

    /** Called every frame. Used to define ImGui behavior per layer. */
    virtual void OnImGuiRender(){};

    /**
     * Propagation of events.
     * @param event Received event.
     */
    virtual void OnEvent(Event& event){};

    /**
     * Getter for layer name.
     * @returns std::string with layer name.
     */
    [[nodiscard]] inline const std::string& GetName() const { return debugName; }

protected:
    std::string debugName;
};
}  // namespace de
