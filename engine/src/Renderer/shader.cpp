#include "Renderer/shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Core/core.h"

namespace de {
Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const auto* source = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, nullptr);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        LOG_ENGINE_ERROR(infoLog.data());
        DE_ASSERT(0, "VertexShader not initialized!")
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        LOG_ENGINE_ERROR(infoLog.data());
        DE_ASSERT(0, "FragmentShader not initialized!")
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_rendererId = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_rendererId, vertexShader);
    glAttachShader(m_rendererId, fragmentShader);

    // Link our program
    glLinkProgram(m_rendererId);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_rendererId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_rendererId, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_rendererId);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        LOG_ENGINE_ERROR(infoLog.data());
        DE_ASSERT(0, "ProgramShader not initialized!")
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_rendererId, vertexShader);
    glDetachShader(m_rendererId, fragmentShader);
}

Shader::~Shader() { glDeleteProgram(m_rendererId); }

void Shader::Bind() const { glUseProgram(m_rendererId); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
    int _location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(matrix));
}
}  // namespace de
