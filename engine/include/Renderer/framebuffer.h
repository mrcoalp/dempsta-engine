#pragma once

#include "Core/core.h"

namespace de {
struct FrameBufferConfig {
    uint32_t width, height;
    uint32_t samples = 1;
    [[nodiscard]] inline float GetAspectRatio() const { return (float)width / (float)height; }
};

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    static Ref<FrameBuffer> Create(const FrameBufferConfig& config);

    virtual void Bind() = 0;

    virtual void Unbind() = 0;

    [[nodiscard]] virtual const FrameBufferConfig& GetConfig() const = 0;

    [[nodiscard]] virtual uint32_t GetColorAttachment() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
};
}  // namespace de
