#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"
#include "Renderer/subtexture.h"
#include "Scene/scenecamera.h"
#include "Scripting/API/databuffer.h"
#include "Scripting/API/luaentity.h"
#include "Scripting/scriptentity.h"
#include "Utils/fileutils.h"

namespace de {
struct NameComponent {
    std::string Name;

    NameComponent() = default;
    explicit NameComponent(std::string name) : Name(std::move(name)) {}

    explicit operator const std::string&() const noexcept { return Name; }
    explicit operator const char*() const noexcept { return Name.c_str(); }
};

struct TransformComponent {
    glm::mat4 Transform = glm::mat4(1.0f);

    TransformComponent() = default;
    explicit TransformComponent(const glm::mat4& transform) : Transform(transform) {}

    explicit operator glm::mat4&() noexcept { return Transform; }
    explicit operator const glm::mat4&() const noexcept { return Transform; }
};

struct SpriteComponent {
    glm::vec4 Color = glm::vec4(1.0f);
    Ref<SubTexture2D> Texture;

    SpriteComponent() = default;
};

class ScriptComponent {
public:
    Scope<lua::DataBuffer> Data;
    Scope<lua::LuaEntity> EntityRef;

    void ReloadScript() { m_code = FileUtils::ReadFile(m_path); }

    void OnInit() const {
        loadCode();
        SE::CallFunction("OnInit", Data.get());
    }

    void OnUpdate(const TimeStep& ts) const {
        loadCode();
        SE::CallFunction("OnUpdate", Data.get(), (float)ts);
    }

    template <typename T>
    void OnEvent(int eventType, T&& action) const {
        loadCode();
        SE::CallFunction("OnEvent", Data.get(), eventType, action);
    }

    void OnMessage(const std::string& id, lua::DataBuffer* data, const std::string& sender) const {
        loadCode();
        SE::CallFunction("OnMessage", Data.get(), id, data, sender);
    }

    void OnDestroy() { EntityRef.reset(nullptr); }

    ScriptComponent() = default;
    explicit ScriptComponent(std::string path) : m_path(std::move(path)) {}

private:
    std::string m_path{""};
    std::string m_code{""};

    void loadCode() const {
        SE::PushGlobalVariable("this", EntityRef.get());
        SE::RunCode(m_code.c_str());
    }
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
