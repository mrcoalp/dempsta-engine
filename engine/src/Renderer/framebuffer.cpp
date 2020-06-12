#include "Renderer/framebuffer.h"

#include "Platform/OpenGL/openglframebuffer.h"
#include "Renderer/renderer.h"

namespace de {
Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferConfig& config) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            DE_THROW("NONE, for now, is not a valid renderer API!")
        }
        case RendererAPI::API::OpenGL: {
            return CreateRef<OpenGLFrameBuffer>(config);
        }
        default: {
            DE_THROW("Not a valid renderer API was provided!")
        }
    }
}
}  // namespace de
