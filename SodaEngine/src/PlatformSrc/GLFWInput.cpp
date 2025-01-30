#include "SD_PCH.h"

#include "Soda/Input/Input.h"

#include "Soda/_Main/App.h"

#include "GLFW/glfw3.h"

namespace Soda
{
bool Input::IsKeyPressed(int keycode)
{
  auto window =
      static_cast<GLFWwindow *>(App::Get().App::GetWindow().GetTheWindow());

  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseClicked(int button)
{
  auto window =
      static_cast<GLFWwindow *>(App::Get().App::GetWindow().GetTheWindow());

  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePos()
{
  auto window =
      static_cast<GLFWwindow *>(App::Get().App::GetWindow().GetTheWindow());

  double xPos, yPos;
  glfwGetCursorPos(window, &xPos, &yPos);

  return {(float)xPos, (float)yPos};
}
} // namespace Soda