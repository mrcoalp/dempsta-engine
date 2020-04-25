#pragma once

#include "Core/core.h"
#include "Core/pch.h"

namespace de {
class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;

    static Ref<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);
};
}  // namespace de
