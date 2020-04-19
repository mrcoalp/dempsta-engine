#pragma once

#include <glm/glm.hpp>

#include "Core/pch.h"

namespace de {
class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    ~Shader();

    void Bind() const;

    void Unbind() const;

    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
    uint32_t m_rendererId;
};
}  // namespace de
