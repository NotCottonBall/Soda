#include "SD_PCH.h"

#include "RenderCommand.h"

#include "OpenGL/OpenGLRenderer.h"

namespace Soda
{
RenderAPI *RenderCommand::m_renderAPI = new OpenGLRenderer();
}