#pragma once

#include <entity/registry.hpp>

#include "Core/timestep.h"
#include "Scene/components.h"

namespace de {
using Entity = entt::entity;

class Scene {
public:
    Scene();

    ~Scene() = default;

    void OnUpdate(const TimeStep& ts);

    [[nodiscard]] inline Entity CreateEntity() noexcept { return m_registry.create(); }

    // NOTE(mpinto): Temp method for testing first implementation
    [[nodiscard]] inline entt::registry& GetReg() noexcept { return m_registry; }

private:
    entt::registry m_registry;
};
}  // namespace de
