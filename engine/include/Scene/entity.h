#pragma once

#include "Core/core.h"
#include "Scene/scene.h"

namespace de {
/**
 * @brief Wrapper class to abstract all EnTT related logic.
 * Holds a scene pointer to access the registry.
 */
class Entity {
public:
    Entity() = default;

    Entity(const entt::entity& handle, Scene* scene) : m_handle(handle), m_scene(scene) {}

    ~Entity() = default;

    /**
     * @brief Adds a new component to Entity.
     *
     * @warning If entity already has component, an exception is thrown.
     *
     * @tparam Component Component to be added.
     * @tparam Args Type of arguments of component.
     * @param args Arguments to construct the component.
     * @return Component& Returns a reference to the component.
     */
    template <typename Component, typename... Args>
    Component& AddComponent(Args&&... args) {
        DE_ASSERT(!HasComponent<Component>(), "Tried to add an already existent component to entity!");
        return m_scene->m_registry.emplace<Component>(m_handle, std::forward<Args>(args)...);
    }

    /**
     * @brief Removes a list of components from the entity.
     *
     * @warning If entity does not have any of the components, an exception is thrown.
     *
     * @tparam Component Component list to remove.
     */
    template <typename... Component>
    void RemoveComponent() {
        DE_ASSERT(HasComponent<Component...>(), "Tried to remove an inexistent component from entity!");
        m_scene->m_registry.remove<Component...>(m_handle);
    }

    /**
     * @brief Checks if entity has a specific list of components.
     *
     * @tparam Component Components to check for.
     * @return true Entity has all the specified components.
     * @return false Entity doesn't have all the specified components.
     */
    template <typename... Component>
    bool HasComponent() {
        return m_scene->m_registry.has<Component...>(m_handle);
    }

    /**
     * @brief Gets components from entity. One or multiple as tuple.
     *
     * @warning If entity does not have component, an exception is thrown.
     *
     * @tparam Component Components to get.
     * @return Returns references to the components.
     */
    template <typename... Component>
    decltype(auto) GetComponent() {
        DE_ASSERT(HasComponent<Component...>(), "Tried to get an inexistent component from entity!");
        return m_scene->m_registry.get<Component...>(m_handle);
    }

    /**
     * @brief Checks if entity is a valid one.
     *
     * @return true Valid entity.
     * @return false Invalid entity.
     */
    explicit operator bool() const noexcept { return (int)m_handle >= 0; }

private:
    entt::entity m_handle;
    Scene* m_scene;
};
}  // namespace de
