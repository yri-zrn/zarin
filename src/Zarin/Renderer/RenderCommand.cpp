#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace zrn
{

RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

} // namespace zrn
