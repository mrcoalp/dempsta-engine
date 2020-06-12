#include "Renderer/rendercommand.h"

#include "Platform/OpenGL/openglrendererapi.h"

namespace de {
Ref<RendererAPI> RenderCommand::s_rendererApi = de::CreateRef<OpenGLRendererAPI>();
}
