#include "Core/Core.h"
#include "SD_PCH.h"


#include "RenderAPI.h"

#include "Renderer/OpenGL/OpenGLShader.h"
#include "Shaderer.h"


namespace Soda
{
Shader *Shader::Create(const std::string &filepath)
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    return new OpenGLShader(filepath);
  }
}

Shader *Shader::Create(const std::string &vertexSrc,
                       const std::string &fragmentSrc)
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    return new OpenGLShader(vertexSrc, fragmentSrc);
  }
}
} // namespace Soda