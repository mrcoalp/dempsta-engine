#pragma once

#include "Core/core.h"
#include "Scene/scene.h"

namespace lua {
class LuaEntity;
}

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
     * @return Returns a reference to the component.
     */
    template <typename Component, typename... Args>
    Component& AddComponent(Args&&... args) {
        DE_ASSERT(!HasComponent<Component>(), "Tried to add an already existent component to entity!");
        return m_scene->m_registry.emplace<Component>(m_handle, std::forward<Args>(args)...);
    }

    /**
     * @brief Remove component from the entity.
     *
     * @warning If entity does not have component, an exception is thrown.
     *
     * @tparam Component Component to remove.
     */
    template <typename Component>
    void RemoveComponent() {
        DE_ASSERT(HasComponent<Component>(), "Tried to remove an inexistent component from entity!");
        m_scene->m_registry.remove<Component>(m_handle);
    }

    /**
     * @brief Checks if entity has a specific component.
     *
     * @tparam Component Component to check for.
     * @return true Entity has the specified component.
     * @return false Entity doesn't have the specified component.
     */
    template <typename Component>
    bool HasComponent() {
        return m_scene->m_registry.has<Component>(m_handle);
    }

    /**
     * @brief Gets component from entity.
     *
     * @warning If entity doesn't have the component, an exception is thrown.
     *
     * @tparam Component Component to get.
     * @return Returns reference to the component.
     */
    template <typename Component>
    Component& GetComponent() {
        DE_ASSERT(HasComponent<Component>(), "Tried to get an inexistent component from entity!");
        return m_scene->m_registry.get<Component>(m_handle);
    }

    /**
     * @brief Checks if entity is a valid one.
     *
     * @return true Valid entity.
     * @return false Invalid entity.
     */
    explicit operator bool() const noexcept { return m_handle != entt::null && m_scene != nullptr; }

    /**
     * @brief Compares if two entities are equal.
     *
     * @param other Entity to compare to.
     * @return true Entities are equal.
     * @return false Entities are not equal.
     */
    bool operator==(const Entity& other) const noexcept {
        return m_handle == other.m_handle && m_scene == other.m_scene;
    }

    /**
     * @brief Compares if two entities are different.
     *
     * @param other Entity to compare to.
     * @return true Entities are different.
     * @return false Entities are not different.
     */
    bool operator!=(const Entity& other) const noexcept { return !(*(this) == other); }

    explicit operator unsigned() const { return static_cast<unsigned>(m_handle); }

private:
    entt::entity m_handle{entt::null};
    Scene* m_scene;
    friend class lua::LuaEntity;
};
}  // namespace de
