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
     * @tparam T Component to be added.
     * @tparam Args Type of arguments of component.
     * @param args Arguments to construct the component.
     * @return T& Returns a reference to the component.
     */
    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        DE_ASSERT(!HasComponent<T>(), "Tried to add an already existent component to entity!");
        return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }

    /**
     * @brief Removes a component from the entity.
     *
     * @tparam T Component to remove.
     */
    template <typename T>
    void RemoveComponent() {
        DE_ASSERT(HasComponent<T>(), "Tried to remove an inexistent component from entity!");
        m_scene->m_registry.remove<T>(m_handle);
    }

    /**
     * @brief Checks i entity has a specific component.
     *
     * @tparam T Component to check for.
     * @return true Entity has the specified component.
     * @return false Entity doesn't have the specified component.
     */
    template <typename T>
    bool HasComponent() {
        return m_scene->m_registry.has<T>(m_handle);
    }

    /**
     * @brief Get the Component object
     *
     * @tparam T Component to get.
     * @return T& Returns a reference to the component.
     */
    template <typename T>
    T& GetComponent() {
        DE_ASSERT(HasComponent<T>(), "Tried to get an inexistent component from entity!");
        return m_scene->m_registry.get<T>(m_handle);
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
