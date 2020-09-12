#include "Platform/OpenGL/openglshader.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "Core/core.h"
#include "Utils/fileutils.h"

namespace de {
static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == VERTEX_SHADER) {
        return GL_VERTEX_SHADER;
    }
    if (type == TESS_CONTROL_SHADER) {
        return GL_TESS_CONTROL_SHADER;
    }
    if (type == TESS_EVALUATION_SHADER) {
        return GL_TESS_EVALUATION_SHADER;
    }
    if (type == GEOMETRY_SHADER) {
        return GL_GEOMETRY_SHADER;
    }
    if (type == FRAGMENT_SHADER) {
        return GL_FRAGMENT_SHADER;
    }
    if (type == COMPUTE_SHADER) {
        return GL_COMPUTE_SHADER;
    }
    LOG_ENGINE_ERROR("Invalid shader type: {0}!", type);
    return 0;
}

static std::string ShaderTypeToString(const GLenum& type) {
    switch (type) {
        case GL_VERTEX_SHADER:
            return VERTEX_SHADER;
        case GL_TESS_CONTROL_SHADER:
            return TESS_CONTROL_SHADER;
        case GL_TESS_EVALUATION_SHADER:
            return TESS_EVALUATION_SHADER;
        case GL_GEOMETRY_SHADER:
            return GEOMETRY_SHADER;
        case GL_FRAGMENT_SHADER:
            return FRAGMENT_SHADER;
        case GL_COMPUTE_SHADER:
            return COMPUTE_SHADER;
        default: {
            LOG_ENGINE_ERROR("Invalid shader type: {0}!", type);
            return "";
        }
    }
}

OpenGLShader::OpenGLShader(const std::string& filepath) {
    m_name = FileUtils::GetFileName(filepath);
    compile(processFileData(FileUtils::ReadFile(filepath)));
}

OpenGLShader::OpenGLShader(std::string name, const std::string& vertexSource, const std::string& fragmentSource)
    : m_name(std::move(name)) {
    std::unordered_map<GLenum, std::string> sources;
    sources.emplace(GL_VERTEX_SHADER, vertexSource);
    sources.emplace(GL_FRAGMENT_SHADER, fragmentSource);
    compile(sources);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_rendererId); }

void OpenGLShader::Bind() const { glUseProgram(m_rendererId); }

void OpenGLShader::Unbind() const { glUseProgram(0); }

void OpenGLShader::UploadUniformInt(const std::string& name, int value) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformVec(const std::string& name, float value) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformVec2(const std::string& name, const glm::vec2& values) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void OpenGLShader::UploadUniformVec3(const std::string& name, const glm::vec3& values) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformVec4(const std::string& name, const glm::vec4& values) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const {
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& sources) {
    std::vector<GLuint> shaders;
    shaders.reserve(sources.size());

    for (const auto& source : sources) {
        // Create an empty shader handle
        GLuint shader = glCreateShader(source.first);

        // Send the shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const auto* code = source.second.c_str();
        glShaderSource(shader, 1, &code, nullptr);

        // Compile the shader
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(shader);

            // Use the infoLog as you see fit.
            DE_THROW("{0} shader not initialized: '{1}'", ShaderTypeToString(source.first), infoLog.data())
        }

        // Add shader id to vector
        shaders.emplace_back(shader);
    }

    // Shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object and assign as your renderer id
    m_rendererId = glCreateProgram();

    // Attach our shaders to our program
    for (const auto& shader : shaders) {
        glAttachShader(m_rendererId, shader);
    }

    // Link our program
    glLinkProgram(m_rendererId);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint _isLinked = 0;
    glGetProgramiv(m_rendererId, GL_LINK_STATUS, &_isLinked);

    if (_isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_rendererId, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_rendererId);
        // Don't leak shaders either.
        for (const auto& shader : shaders) {
            glDeleteShader(shader);
        }

        DE_THROW("ProgramShader not initialized: '{0}'", infoLog.data())
    }

    // Always detach shaders after a successful link.
    for (const auto& shader : shaders) {
        glDetachShader(m_rendererId, shader);
    }
}

std::unordered_map<GLenum, std::string> OpenGLShader::processFileData(const std::string& fileSource) {
    std::unordered_map<GLenum, std::string> sources;
    constexpr const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t position = fileSource.find(typeToken, 0);

    while (position != std::string::npos) {
        size_t _eol = fileSource.find_first_of('\n', position);
        DE_ASSERT(_eol != std::string::npos, "Syntax error!")

        size_t _begin = position + typeTokenLength + 1;
        std::string type = fileSource.substr(_begin, _eol - _begin);
        DE_ASSERT(ShaderTypeFromString(type), "Invalid shader type: '{0}'", type)

        size_t nextLinePosition = fileSource.find_first_of('\n', _eol);
        position = fileSource.find(typeToken, nextLinePosition);
        size_t endOfSourcePosition =
            nextLinePosition == std::string::npos ? fileSource.size() - 1 : nextLinePosition;
        std::string _source = fileSource.substr(nextLinePosition, position - endOfSourcePosition);

        sources.emplace(ShaderTypeFromString(type), _source);
    }

    return sources;
}
void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) { UploadUniformMat4(name, value); }

void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value) { UploadUniformVec4(name, value); }

void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value) { UploadUniformVec3(name, value); }

void OpenGLShader::SetInt(const std::string& name, int value) { UploadUniformInt(name, value); }

void OpenGLShader::SetIntArray(const std::string& name, const int* values, uint32_t count) {
    UploadUniformIntArray(name, values, count);
}
}  // namespace de
