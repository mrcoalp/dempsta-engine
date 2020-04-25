#pragma once

#include "Core/core.h"

namespace de {
class Texture {
public:
    ~Texture() = default;

    [[nodiscard]] virtual unsigned GetWidth() const = 0;

    [[nodiscard]] virtual unsigned GetHeight() const = 0;

    virtual void Bind(unsigned slot = 0) const = 0;
};

class Texture2D : public Texture {
public:
    static Ref<Texture2D> Create(const std::string& filePath);
};
}  // namespace de
