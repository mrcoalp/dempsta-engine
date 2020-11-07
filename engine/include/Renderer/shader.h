#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"

namespace de {
class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;

    virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

    virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;

    virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;

    virtual void SetInt(const std::string& name, int value) = 0;

    virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) = 0;

    [[nodiscard]] virtual const std::string& GetName() const = 0;

    static Ref<Shader> Create(const std::string& filePath);

    static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
};
}  // namespace de
