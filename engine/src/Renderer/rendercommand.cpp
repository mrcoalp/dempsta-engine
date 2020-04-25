#include "Renderer/rendercommand.h"

#include "Platform/OpenGL/openglrendererapi.h"

namespace de {
Ref<RendererAPI> RenderCommand::s_rendererApi = std::make_shared<OpenGLRendererAPI>();
}
