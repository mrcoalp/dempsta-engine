#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/core.h"
#include "Renderer/label.h"
#include "Renderer/subtexture.h"
#include "Scene/scenecamera.h"
#include "Scripting/scriptentity.h"
#include "Sound/sound.h"

namespace de {
struct NameComponent {
    std::string name;

    NameComponent() = default;
    explicit NameComponent(std::string name) : name(std::move(name)) {}

    explicit operator const std::string &() const noexcept { return name; }
    explicit operator const char*() const noexcept { return name.c_str(); }
};

struct TransformComponent {
    glm::vec3 translation{0.f, 0.f, 0.f};
    glm::vec3 rotation{0.f, 0.f, 0.f};
    glm::vec3 scale{1.f, 1.f, 1.f};

    [[nodiscard]] glm::mat4 GetTransform() const {
        auto rotationX = glm::rotate(glm::mat4(1.f), rotation.x, {1.f, 0.f, 0.f});
        auto rotationY = glm::rotate(glm::mat4(1.f), rotation.y, {0.f, 1.f, 0.f});
        auto rotationZ = glm::rotate(glm::mat4(1.f), rotation.z, {0.f, 0.f, 1.f});
        return glm::translate(glm::mat4(1.f), translation) * rotationX * rotationY * rotationZ * glm::scale(glm::mat4(1.f), scale);
    }

    explicit operator glm::mat4() const noexcept { return GetTransform(); }

    TransformComponent() = default;
    explicit TransformComponent(const glm::vec3& translation) : translation(translation) {}
};

struct SpriteComponent {
    glm::vec4 color{glm::vec4(1.0f)};
    glm::vec2 anchor{glm::vec2(0.f)};
    Ref<SubTexture2D> texture;

    SpriteComponent() = default;
};

struct ScriptComponent {
    std::string path;
    Scope<lua::ScriptEntity> instance;

    void Create() { instance = CreateScope<lua::ScriptEntity>(path); }

    void Destroy() {
        instance->OnDestroy();
        instance.reset(nullptr);
    }

    ScriptComponent() = default;
    explicit ScriptComponent(std::string path) : path(std::move(path)) {}
};

struct NativeScriptComponent {
    NativeScriptEntity* instance = nullptr;

    NativeScriptEntity* (*Create)();
    void (*Destroy)(NativeScriptComponent*);

    template <class Script>
    void Bind() {
        Create = []() { return dynamic_cast<NativeScriptEntity*>(new Script()); };
        Destroy = [](NativeScriptComponent* nsc) {
            delete nsc->instance;
            nsc->instance = nullptr;
        };
    }

    NativeScriptComponent() = default;
};

struct CameraComponent {
    SceneCamera camera;
    bool primary = false;
    bool fixedAspectRatio = false;

    CameraComponent() = default;
};

struct LabelComponent {
    Ref<Label> label;
    glm::vec4 color = glm::vec4(1.0f);

    LabelComponent() = default;
};

struct SoundComponent {
    Ref<SoundInstance> sound;

    SoundComponent() = default;
    explicit SoundComponent(const std::string& filePath) : sound(SoundInstance::CreateSound(filePath)) {}
};
}  // namespace de
