#pragma once

#include "Core/pch.h"
#include "Core/timestep.h"
#include "Events/event.h"

namespace de {
class Layer {
public:
    explicit Layer(std::string name = "Layer") : debugName(std::move(name)) {}

    virtual ~Layer() = default;

    virtual void OnAttach(){};

    virtual void OnDetach(){};

    virtual void OnUpdate(const TimeStep& ts){};

    virtual void OnImGuiRender(){};

    virtual void OnEvent(Event& event){};

    [[nodiscard]] inline const std::string& GetName() const { return debugName; }

protected:
    std::string debugName;
};
}  // namespace de
