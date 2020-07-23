#pragma once

#include <entity/registry.hpp>

#include "Core/timestep.h"
#include "Scene/components.h"

namespace de {
class Entity;

/**
 * @brief Handles all scene related stuff.
 */
class Scene {
public:
    Scene() = default;

    ~Scene() = default;

    /**
     * @brief Called every frame.
     *
     * @param ts Delta time.
     */
    void OnUpdate(const TimeStep& ts);

    /**
     * @brief Creates a Entity object
     *
     * @param name Name of the entity. Adds a new NameComponent. Default = "New Entity"
     * @return Entity
     */
    Entity CreateEntity(const std::string& name = "New Entity");

private:
    /**
     * @brief Storage for all the entities. Handled by EnTT.
     */
    entt::registry m_registry;

    friend class Entity;
};
}  // namespace de
