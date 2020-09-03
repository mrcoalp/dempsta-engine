#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"
#include "Scene/scenecamera.h"
#include "Scripting/API/databuffer.h"
#include "Scripting/API/luaentity.h"
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

    SpriteComponent() = default;
    explicit SpriteComponent(const glm::vec4& color) : Color(color) {}
};

struct ScriptComponent {
    std::string Path;
    Scope<lua::DataBuffer> Data;
    Scope<lua::LuaEntity> EntityRef;

    void LoadScript() const {
        SE::LoadFile(Path.c_str());
        SE::PushGlobalVariable("this", EntityRef.get());
    }

    void OnInit() const {
        LoadScript();
        SE::CallFunction("OnInit", Data.get());
    }

    void OnUpdate(const TimeStep& ts) const {
        LoadScript();
        SE::CallFunction("OnUpdate", Data.get(), (float)ts);
    }

    void OnDestroy() { EntityRef.reset(nullptr); }

    ScriptComponent() = default;
    explicit ScriptComponent(std::string path) : Path(std::move(path)) {}
};

struct NativeScriptComponent {
    ScriptEntity* Instance = nullptr;

    ScriptEntity* (*Create)();
    void (*Destroy)(NativeScriptComponent*);

    template <class Script>
    void Bind() {
        Create = []() { return dynamic_cast<ScriptEntity*>(new Script()); };
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
