#pragma once

#include "Core/pch.h"

namespace de {
    class Shader {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);

        ~Shader();

        void Bind() const;

        void Unbind() const;

    private:
        uint32_t rendererId;
    };
}
