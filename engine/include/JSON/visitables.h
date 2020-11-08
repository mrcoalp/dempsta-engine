#pragma once

#include "JSON/json.h"

namespace de::JSON {
struct Vec2 : public Visitable {
    float x;
    float y;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Vec2>& visitor) {
        visitor.Node(x, "x").Node(y, "y");
    }

    [[nodiscard]] inline glm::vec2 ToGLM() const { return {x, y}; }

    Vec2() = default;
    explicit Vec2(const glm::vec2& vec) : x(vec.x), y(vec.y) {}
};

struct Vec3 : public Visitable {
    float x;
    float y;
    float z;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Vec3>& visitor) {
        visitor.Node(x, "x").Node(y, "y").Node(z, "z");
    }

    [[nodiscard]] inline glm::vec3 ToGLM() const { return {x, y, z}; }

    Vec3() = default;
    explicit Vec3(const glm::vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}
};

struct Vec4 : public Visitable {
    float x;
    float y;
    float z;
    float w;

    template <template <typename> class Visitor>
    inline void Visit(Visitor<Vec4>& visitor) {
        visitor.Node(x, "x").Node(y, "y").Node(z, "z").Node(w, "w");
    }

    [[nodiscard]] inline glm::vec4 ToGLM() const { return {x, y, z, w}; }

    Vec4() = default;
    explicit Vec4(const glm::vec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
};
}  // namespace de::JSON
