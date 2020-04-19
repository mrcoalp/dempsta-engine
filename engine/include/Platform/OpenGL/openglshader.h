#pragma once

#include <glm/glm.hpp>

#include "Renderer/shader.h"

namespace de {
class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);

    ~OpenGLShader() override;

    void Bind() const override;

    void Unbind() const override;

    void UploadUniformInt(const std::string& name, int value) const;

    void UploadUniformVec(const std::string& name, float value) const;
    void UploadUniformVec2(const std::string& name, const glm::vec2& values) const;
    void UploadUniformVec3(const std::string& name, const glm::vec3& values) const;
    void UploadUniformVec4(const std::string& name, const glm::vec4& values) const;

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    uint32_t m_rendererId;
};
}  // namespace de
