#pragma once

#include <glm/glm.hpp>

#include "Core/core.h"

namespace de {
class Texture {
public:
    virtual ~Texture() = default;

    [[nodiscard]] virtual unsigned GetWidth() const = 0;

    [[nodiscard]] virtual unsigned GetHeight() const = 0;

    virtual void SetData(void* data, const glm::vec2& offset = glm::vec2(0.f)) = 0;

    virtual void SetData(void* data, const glm::vec2& offset, unsigned width, unsigned height) = 0;

    virtual void Bind(unsigned slot) const = 0;

    [[nodiscard]] virtual bool Equals(const Texture& other) const = 0;

    [[nodiscard]] virtual uint32_t GetRendererID() const = 0;
};

class Texture2D : public Texture {
public:
    static Ref<Texture2D> Create(uint32_t width, uint32_t height);
    static Ref<Texture2D> Create(const std::string& filePath);
};
}  // namespace de
