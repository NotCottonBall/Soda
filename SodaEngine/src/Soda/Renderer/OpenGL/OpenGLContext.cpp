#include "SD_PCH.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "OpenGLContext.h"

namespace Soda
{
void SodaGL::Init()
{
  // initializing GLAD
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  SD_ENGINE_ASSERT(status, "Failed to initialize GLAD!");

  SD_ENGINE_LOG("Initialized Rendering Context With:"
                "\n\tGPU: {0}"
                "\n\tGL_VERSION: {1}",
                (const char *)glGetString(GL_RENDERER),
                (const char *)glGetString(GL_VERSION));
}
} // namespace Soda