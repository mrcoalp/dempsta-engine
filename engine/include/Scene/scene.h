#pragma once

#include <entity/registry.hpp>

#include "Core/timestep.h"
#include "Scene/components.h"

namespace de {
class Entity;

class Scene {
public:
    Scene();

    ~Scene() = default;

    void OnUpdate(const TimeStep& ts);

    Entity CreateEntity(const std::string& name = "New Entity");

private:
    entt::registry m_registry;

    friend class Entity;
};
}  // namespace de
