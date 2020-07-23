#pragma once

#include <glm/glm.hpp>

namespace de {
struct NameComponent {
    std::string Name;

    NameComponent() = default;
    NameComponent(const NameComponent&) = default;
    explicit NameComponent(std::string name) : Name(std::move(name)) {}

    explicit operator const std::string &() const noexcept { return Name; }
    explicit operator const char*() const noexcept { return Name.c_str(); }
};

struct TransformComponent {
    glm::mat4 Transform = glm::mat4(1.0f);

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    explicit TransformComponent(const glm::mat4& transform) : Transform(transform) {}

    explicit operator glm::mat4 &() noexcept { return Transform; }
    explicit operator const glm::mat4 &() const noexcept { return Transform; }
};

struct SpriteComponent {
    glm::vec4 Color = glm::vec4(1.0f);

    SpriteComponent() = default;
    SpriteComponent(const SpriteComponent&) = default;
    explicit SpriteComponent(const glm::vec4& color) : Color(color) {}
};
}  // namespace de
