#pragma once

#include "Core/core.h"
#include "Scene/scene.h"

namespace de {
class Entity {
public:
    Entity() = default;

    Entity(const entt::entity& handle, Scene* scene) : m_handle(handle), m_scene(scene) {}

    ~Entity() = default;

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        DE_ASSERT(!HasComponent<T>(), "Tried to add an already existent component to entity!");
        return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }

    template <typename T>
    void RemoveComponent() {
        DE_ASSERT(HasComponent<T>(), "Tried to remove an inexistent component from entity!");
        m_scene->m_registry.remove<T>(m_handle);
    }

    template <typename T>
    bool HasComponent() {
        return m_scene->m_registry.has<T>(m_handle);
    }

    template <typename T>
    T& GetComponent() {
        DE_ASSERT(HasComponent<T>(), "Tried to get an inexistent component from entity!");
        return m_scene->m_registry.get<T>(m_handle);
    }

    explicit operator bool() const noexcept { return (int)m_handle >= 0; }

private:
    entt::entity m_handle{0};
    Scene* m_scene{nullptr};
};
}  // namespace de
