#include "SD_PCH.h"
#include "Soda/_Main/Core.h"

#include "RenderAPI.h"

#include "Shaderer.h"
#include "Soda/Renderer/OpenGL/OpenGLShader.h"

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