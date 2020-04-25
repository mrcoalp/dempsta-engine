#include "Platform/OpenGL/openglshader.h"

#include <glm/gtc/type_ptr.hpp>

#include "Core/core.h"
#include "Utils/fileutils.h"

namespace de {
static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == VERTEX_SHADER) {
        return GL_VERTEX_SHADER;
    } else if (type == TESS_CONTROL_SHADER) {
        return GL_TESS_CONTROL_SHADER;
    } else if (type == TESS_EVALUATION_SHADER) {
        return GL_TESS_EVALUATION_SHADER;
    } else if (type == GEOMETRY_SHADER) {
        return GL_GEOMETRY_SHADER;
    } else if (type == FRAGMENT_SHADER) {
        return GL_FRAGMENT_SHADER;
    } else if (type == COMPUTE_SHADER) {
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

OpenGLShader::OpenGLShader(const std::string& filepath) { compile(processFileData(FileUtils::ReadFile(filepath))); }

OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource) {
    std::unordered_map<GLenum, std::string> _sources;
    _sources.emplace(GL_VERTEX_SHADER, vertexSource);
    _sources.emplace(GL_FRAGMENT_SHADER, fragmentSource);
    compile(_sources);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_rendererId); }

void OpenGLShader::Bind() const { glUseProgram(m_rendererId); }

void OpenGLShader::Unbind() const { glUseProgram(0); }

void OpenGLShader::UploadUniformInt(const std::string& name, int value) const {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1i(_location, value);
}

void OpenGLShader::UploadUniformVec(const std::string& name, float value) const {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1f(_location, value);
}

void OpenGLShader::UploadUniformVec2(const std::string& name, const glm::vec2& values) const {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform2f(_location, values.x, values.y);
}

void OpenGLShader::UploadUniformVec3(const std::string& name, const glm::vec3& values) const {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform3f(_location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformVec4(const std::string& name, const glm::vec4& values) const {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform4f(_location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix3fv(_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& sources) {
    std::vector<GLuint> _shaders;

    for (const auto& source : sources) {
        // Create an empty shader handle
        GLuint _shader = glCreateShader(source.first);

        // Send the shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const auto* _source = source.second.c_str();
        glShaderSource(_shader, 1, &_source, nullptr);

        // Compile the shader
        glCompileShader(_shader);

        GLint _isCompiled = 0;
        glGetShaderiv(_shader, GL_COMPILE_STATUS, &_isCompiled);

        if (_isCompiled == GL_FALSE) {
            GLint _maxLength = 0;
            glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &_maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> _infoLog(_maxLength);
            glGetShaderInfoLog(_shader, _maxLength, &_maxLength, &_infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(_shader);

            // Use the infoLog as you see fit.
            LOG_ENGINE_ERROR(_infoLog.data());
            DE_ASSERT(0, "{0} not initialized!", ShaderTypeToString(source.first))
        }

        // Add shader id to vector
        _shaders.push_back(_shader);
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_rendererId = glCreateProgram();

    for (const auto& shader : _shaders) {
        // Attach our shaders to our program
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
        for (const auto& shader : _shaders) {
            glDeleteShader(shader);
        }

        LOG_ENGINE_ERROR(infoLog.data());
        DE_ASSERT(0, "ProgramShader not initialized!")
    }

    // Always detach shaders after a successful link.
    for (const auto& shader : _shaders) {
        glDetachShader(m_rendererId, shader);
    }
}

std::unordered_map<GLenum, std::string> OpenGLShader::processFileData(const std::string& fileSource) {
    std::unordered_map<GLenum, std::string> _sources;
    const char* _typeToken = "#type";
    size_t _typeTokenLength = strlen(_typeToken);
    size_t _position = fileSource.find(_typeToken, 0);

    while (_position != std::string::npos) {
        size_t _eol = fileSource.find_first_of('\n', _position);
        DE_ASSERT(_eol != std::string::npos, "Syntax error!")

        size_t _begin = _position + _typeTokenLength + 1;
        std::string _type = fileSource.substr(_begin, _eol - _begin);
        DE_ASSERT(ShaderTypeFromString(_type), "Invalid shader type: {0}", _type)

        size_t _nextLinePosition = fileSource.find_first_of('\n', _eol);
        _position = fileSource.find(_typeToken, _nextLinePosition);
        size_t _endOfSourcePosition =
            _nextLinePosition == std::string::npos ? fileSource.size() - 1 : _nextLinePosition;
        std::string _source = fileSource.substr(_nextLinePosition, _position - _endOfSourcePosition);

        _sources.emplace(ShaderTypeFromString(_type), _source);
    }

    return _sources;
}
}  // namespace de
