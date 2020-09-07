#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"
#include "Renderer/subtexture.h"
#include "Scene/scenecamera.h"
#include "Scripting/scriptentity.h"

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
    Ref<SubTexture2D> Texture;

    SpriteComponent() = default;
};

struct ScriptComponent {
    std::string Path;
    Scope<lua::ScriptEntity> Instance;

    void Create() { Instance.reset(new lua::ScriptEntity(Path)); }

    void Destroy() {
        Instance->OnDestroy();
        Instance.reset(nullptr);
    }

    ScriptComponent() = default;
    explicit ScriptComponent(std::string path) : Path(std::move(path)) {}
};

struct NativeScriptComponent {
    NativeScriptEntity* Instance = nullptr;

    NativeScriptEntity* (*Create)();
    void (*Destroy)(NativeScriptComponent*);

    template <class Script>
    void Bind() {
        Create = []() { return dynamic_cast<NativeScriptEntity*>(new Script()); };
        Destroy = [](NativeScriptComponent* nsc) {
            delete nsc->Instance;
            nsc->Instance = nullptr;
        };
    }

    NativeScriptComponent() = default;
};

struct CameraComponent {
    SceneCamera Camera;
    bool Primary = false;
    bool FixedAspectRatio = false;

    CameraComponent() = default;
};
}  // namespace de
