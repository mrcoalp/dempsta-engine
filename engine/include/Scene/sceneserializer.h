#pragma once

#include "Core/core.h"
#include "Scene/scene.h"

namespace de {
class SceneSerializer {
public:
    explicit SceneSerializer(const Ref<Scene>& scene) : m_scene(scene) {}

    void Serialize(const std::string& filePath) const;

    [[nodiscard]] bool Deserialize(const std::string& filePath) const;

private:
    Ref<Scene> m_scene;
};
}  // namespace de
