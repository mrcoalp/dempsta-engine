#include "Renderer/rendercommand.h"

#include "Platform/OpenGL/openglrendererapi.h"

namespace de {
RendererAPI* RenderCommand::s_rendererApi = new OpenGLRendererAPI();
}
