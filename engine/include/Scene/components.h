#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"
#include "Renderer/camera.h"
#include "Scripting/API/databuffer.h"

namespace de {
struct NameComponent {
    std::string Name;

    NameComponent() = default;
    explicit NameComponent(std::string name) : Name(std::move(name)) {}

    explicit operator const std::string &() const noexcept { return Name; }
    explicit operator const char*() const noexcept { return Name.c_str(); }
};

struct TransformComponent {
    glm::mat4 Transform = glm::mat4(1.0f);

    TransformComponent() = default;
    explicit TransformComponent(const glm::mat4& transform) : Transform(transform) {}

    explicit operator glm::mat4 &() noexcept { return Transform; }
    explicit operator const glm::mat4 &() const noexcept { return Transform; }
};

struct SpriteComponent {
    glm::vec4 Color = glm::vec4(1.0f);

    SpriteComponent() = default;
    explicit SpriteComponent(const glm::vec4& color) : Color(color) {}
};

struct ScriptComponent {
    std::string Path;
    Ref<lua::DataBuffer> Data = CreateRef<lua::DataBuffer>();

    ScriptComponent() = default;
    explicit ScriptComponent(std::string path) : Path(std::move(path)) {
        SE::LoadFile(Path.c_str());
        SE::CallFunction("OnInit", Data.get());
    }
};

struct CameraComponent {
    Camera Cam = Camera(glm::mat4(1.0));
    bool Primary = false;

    CameraComponent() = default;
    explicit CameraComponent(const glm::mat4& projection, bool primary = false)
        : Cam(Camera(projection)), Primary(primary) {}
};
}  // namespace de
