#pragma once

#include "Core/core.h"
#include "Core/pch.h"

namespace de {
class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;

    [[nodiscard]] virtual const std::string& GetName() const = 0;

    static Ref<Shader> Create(const std::string& filePath);

    static Ref<Shader> Create(const std::string& name, const std::string& vertexSource,
                              const std::string& fragmentSource);
};

class ShaderLibrary {
public:
    void Add(const Ref<Shader>& shader);

    void Add(const std::string& name, const Ref<Shader>& shader);

    Ref<Shader> Load(const std::string& filepath);

    Ref<Shader> Load(const std::string& name, const std::string& filepath);

    const Ref<Shader>& Get(const std::string& name) const;

    inline bool Exists(const std::string& name) const { return m_shaders.find(name) != m_shaders.end(); }

private:
    std::unordered_map<std::string, Ref<Shader>> m_shaders;
};
}  // namespace de
