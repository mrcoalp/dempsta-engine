#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "Renderer/shader.h"

#define VERTEX_SHADER "vertex"
#define TESS_CONTROL_SHADER "tess_control"
#define TESS_EVALUATION_SHADER "tess_evaluation"
#define GEOMETRY_SHADER "geometry"
#define FRAGMENT_SHADER "fragment"
#define COMPUTE_SHADER "compute"

namespace de {
class OpenGLShader : public Shader {
public:
    explicit OpenGLShader(const std::string& filepath);

    OpenGLShader(std::string name, const std::string& vertexSource, const std::string& fragmentSource);

    ~OpenGLShader() final;

    void Bind() const final;

    void Unbind() const final;

    void SetMat4(const std::string& name, const glm::mat4& value) final;
    void SetVec4(const std::string& name, const glm::vec4& value) final;
    void SetVec3(const std::string& name, const glm::vec3& value) final;
    void SetInt(const std::string& name, int value) final;
    void SetIntArray(const std::string& name, const int* values, uint32_t count) final;

    [[nodiscard]] inline const std::string& GetName() const final { return m_name; }

    void UploadUniformInt(const std::string& name, int value) const;
    void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const;

    void UploadUniformVec(const std::string& name, float value) const;
    void UploadUniformVec2(const std::string& name, const glm::vec2& values) const;
    void UploadUniformVec3(const std::string& name, const glm::vec3& values) const;
    void UploadUniformVec4(const std::string& name, const glm::vec4& values) const;

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    uint32_t m_rendererId;
    std::string m_name;

    void compile(const std::unordered_map<GLenum, std::string>& sources);

    static std::unordered_map<GLenum, std::string> processFileData(const std::string& fileSource);
};
}  // namespace de
