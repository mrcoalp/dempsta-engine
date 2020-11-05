#pragma once

#include <utility>

#include "Core/core.h"
#include "JSON/json.h"
#include "Scene/scene.h"

namespace de {
namespace JSON {
struct NameComponent : public Visitable {
    bool added = false;
    std::string name;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<NameComponent>& visitor) {
        visitor.Node(added, "added").Node(name, "name");
    }
};

struct TransformComponent : public Visitable {
    bool added = false;
    Vec3 translation;
    Vec3 rotation;
    Vec3 scale;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<TransformComponent>& visitor) {
        visitor.Node(added, "added").Node(translation, "translation").Node(rotation, "rotation").Node(scale, "scale");
    }
};

struct CameraComponent : public Visitable {
    bool added = false;
    bool primary;
    bool fixedAspectRatio;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<CameraComponent>& visitor) {
        visitor.Node(added, "added").Node(primary, "primary").Node(fixedAspectRatio, "fixed_aspect_ratio");
    }
};

struct Entity : public Visitable {
    unsigned id;
    NameComponent nameComponent;
    TransformComponent transformComponent;
    CameraComponent cameraComponent;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Entity>& visitor) {
        visitor.Node(id, "id")
            .OptionalNode(nameComponent, "name_component")
            .OptionalNode(transformComponent, "transform_component")
            .OptionalNode(cameraComponent, "camera_component");
    }
};

struct Scene : public Visitable {
    std::string id;
    std::vector<Entity> entities;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Scene>& visitor) {
        visitor.Node(id, "id").Node(entities, "entities");
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
