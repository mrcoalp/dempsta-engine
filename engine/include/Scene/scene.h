#pragma once

#include <entt/entt.hpp>

#include "Core/timestep.h"
#include "Core/uuid.h"
#include "Events/event.h"

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

    void OnEvent(Event& event);

    /**
     * @brief Handles resize of viewport.
     *
     * @param width Viewport width.
     * @param height Viewport height.
     */
    void OnViewportResize(uint32_t width, uint32_t height);

    template <typename Component>
    void OnAddComponent(Entity entity, Component& component);

    /**
     * @brief Creates a Entity object
     *
     * @param name Name of the entity. Adds a new NameComponent. Default = "New Entity"
     * @return Entity
     */
    Entity CreateEntity(const std::string& name = "New Entity", bool addTransform = true);

    /**
     * @brief Creates a Entity with a specified id
     *
     * @param uuid ID to create entity with
     * @param name Name of the entity. Adds a new NameComponent. Default = "New Entity"
     * @return Entity
     */
    Entity CreateEntityWithID(const UUID& uuid, const std::string& name = "New Entity", bool addTransform = true);

    /**
     * @brief Destroys a given entity
     *
     * @param entity Entity to destroy
     */
    void DestroyEntity(Entity entity);

    /**
     * @brief Iterates through each entity in the registry and applies callback on it.
     *
     * @tparam Callback Type of the callback function to call.
     * @param callback Callback for each entity.
     */
    template <typename Callback>
    inline void ForEachEntity(Callback&& callback) {
        m_registry.each(std::forward<Callback>(callback));
    }

private:
    /**
     * @brief Storage for all the entities. Handled by EnTT.
     */
    entt::registry m_registry;

    uint32_t m_viewportWidth{0};
    uint32_t m_viewportHeight{0};

    friend class Entity;
};
}  // namespace de
