#pragma once

#include <utility>

#include "Core/core.h"
#include "JSON/json.h"
#include "Scene/scene.h"

namespace de {
namespace JSON {
struct NameComponent : public Visitable {
    std::string name;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<NameComponent>& visitor) {
        visitor.Node(name, "name");
    }
};

struct TransformComponent : public Visitable {
    Vec3 translation;
    Vec3 rotation;
    Vec3 scale;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<TransformComponent>& visitor) {
        visitor.Node(translation, "translation").Node(rotation, "rotation").Node(scale, "scale");
    }
};

struct OrthographicCamera : public Visitable {
    float size;
    float nearClip;
    float farClip;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<OrthographicCamera>& visitor) {
        visitor.Node(size, "size").Node(nearClip, "near_clip").Node(farClip, "far_clip");
    }
};

struct SceneCamera : public Visitable {
    OrthographicCamera ortho{true};

    template <template <typename> class Visitor>
    inline void Visit(Visitor<SceneCamera>& visitor) {
        visitor.OptionalNode(ortho, "orthographic", {true});
    }
};

struct CameraComponent : public Visitable {
    bool primary;
    bool fixedAspectRatio;
    SceneCamera sceneCamera;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<CameraComponent>& visitor) {
        visitor.Node(primary, "primary").Node(fixedAspectRatio, "fixed_aspect_ratio").Node(sceneCamera, "scene_camera");
    }
};

struct SpriteComponent : public Visitable {
    Vec4 color;
    Vec2 anchor;
    std::string asset;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<SpriteComponent>& visitor) {
        visitor.Node(color, "color").Node(anchor, "anchor").Node(asset, "asset");
    }
};

struct ScriptComponent : public Visitable {
    std::string asset;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<ScriptComponent>& visitor) {
        visitor.Node(asset, "asset");
    }
};

struct SoundComponent : public Visitable {
    std::string asset;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<SoundComponent>& visitor) {
        visitor.Node(asset, "asset");
    }
};

struct LabelComponent : public Visitable {
    std::string asset;
    std::string content;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<LabelComponent>& visitor) {
        visitor.Node(asset, "asset").Node(content, "content");
    }
};

struct Entity : public Visitable {
    unsigned id;
    // All visitable components are empty by default to ensure they're not added to json
    NameComponent nameComponent{true};
    TransformComponent transformComponent{true};
    CameraComponent cameraComponent{true};
    SpriteComponent spriteComponent{true};
    ScriptComponent scriptComponent{true};
    SoundComponent soundComponent{true};
    LabelComponent labelComponent{true};

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Entity>& visitor) {
        visitor.Node(id, "id")
            .OptionalNode(nameComponent, "name_component", {true})
            .OptionalNode(transformComponent, "transform_component", {true})
            .OptionalNode(cameraComponent, "camera_component", {true})
            .OptionalNode(spriteComponent, "sprite_component", {true})
            .OptionalNode(scriptComponent, "script_component", {true})
            .OptionalNode(soundComponent, "sound_component", {true})
            .OptionalNode(labelComponent, "label_component", {true});
    }
};

struct Asset : public Visitable {
    int type;
    std::string path;
    std::string name;
    unsigned fontSize;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Asset>& visitor) {
        visitor.Node(type, "type").Node(path, "path").Node(name, "name").Node(fontSize, "font_size");
    }
};

struct Scene : public Visitable {
    std::string id;
    std::vector<Asset> assets;
    std::vector<Entity> entities;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Scene>& visitor) {
        visitor.Node(id, "id").Node(assets, "assets").Node(entities, "entities");
    }
};
}  // namespace JSON

class SceneSerializer {
public:
    explicit SceneSerializer(const Ref<Scene>& scene) : m_scene(scene) {}

    void Serialize(const std::string& filePath) const;

    [[nodiscard]] bool Deserialize(const std::string& filePath) const;

private:
    Ref<Scene> m_scene;
};
}  // namespace de
