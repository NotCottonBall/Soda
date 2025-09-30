#include "SD_PCH.h"

#include "Core/App/App.h"
#include "Core/Core.h"

#include "EditorCamera.h"

#include "Input/Input.h"
#include "Input/KeyboardCodes.h"
#include "Input/MouseCodes.h"

namespace Soda
{
EditorCamera::EditorCamera(float aspectRatio)
    : m_aspectRatio(aspectRatio), m_cameraPosition({0.0f, 0.0f, m_zoomLevel})
{
  RecalculateMatrices();
  m_yaw = -90.0f;
  m_pitch = 0.0f;
  m_lastX = App::Get().GetWindow().GetWindowWidth() / 2;
  m_lastY = App::Get().GetWindow().GetWindowHeight() / 2;
  m_firstMouse = true;
}

void EditorCamera::SetProjectionMat(float fov, float aspectRatio,
                                    float nearPlane, float farPlane)
{
  m_Projection =
      glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

void EditorCamera::OnUpdate(float dt)
{
  if(Input::IsMouseClicked(SD_MOUSE_BUTTON_1) ||
     Input::IsKeyPressed(SD_KEY_LEFT_ALT))
  {
    // up and down
    if(Input::IsKeyPressed(SD_KEY_E))
      m_cameraPosition.y += m_cameraMoveSpeed * dt;
    if(Input::IsKeyPressed(SD_KEY_Q))
      m_cameraPosition.y -= m_cameraMoveSpeed * dt;

    // base move dirz
    if(Input::IsKeyPressed(SD_KEY_W))
      m_cameraPosition += (m_forwardVec * m_cameraMoveSpeed) * dt;
    if(Input::IsKeyPressed(SD_KEY_S))
      m_cameraPosition -= (m_forwardVec * m_cameraMoveSpeed) * dt;
    if(Input::IsKeyPressed(SD_KEY_A))
      m_cameraPosition -=
          (glm::cross(m_forwardVec, glm::vec3(0.0f, 1.0f, 0.0f)) *
           m_cameraMoveSpeed) *
          dt;
    if(Input::IsKeyPressed(SD_KEY_D))
      m_cameraPosition +=
          (glm::cross(m_forwardVec, glm::vec3(0.0f, 1.0f, 0.0f)) *
           m_cameraMoveSpeed) *
          dt;
  }

  RecalculateMatrices();
}

void EditorCamera::OnEvent(Event &event)
{
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseScrollEvent>(BIND_FN(EditorCamera::OnMouseScrolled));
  dispatcher.Dispatch<WindowResizeEvent>(
      BIND_FN(EditorCamera::OnWindowResized));
  dispatcher.Dispatch<MouseMoveEvent>(BIND_FN(EditorCamera::OnMouseMove));
}

bool EditorCamera::OnMouseScrolled(MouseScrollEvent &msEvent)
{
  // @TODO: Scrolling Functionality
  m_cameraMoveSpeed = m_zoomLevel;

  m_zoomLevel -= msEvent.GetWheelOffsetY() * 0.25f;
  m_zoomLevel = std::max(m_zoomLevel, 0.25f);

  RecalculateMatrices();

  return false;
}

bool EditorCamera::OnWindowResized(WindowResizeEvent &wrEvent)
{
  OnResize((float)wrEvent.GetWindowWidth(), (float)wrEvent.GetWindowHeight());

  return false;
}
bool EditorCamera::OnMouseMove(MouseMoveEvent &mmEvent)
{
  if(Input::IsMouseClicked(SD_MOUSE_BUTTON_1))
  {
    m_Spectating = true;
    if(m_firstMouse)
    {
      m_lastX = mmEvent.GetMouseX();
      m_lastY = mmEvent.GetMouseY();
      m_firstMouse = false;
    }

    float xOffset = mmEvent.GetMouseX() - m_lastX;
    float yOffset = m_lastY - mmEvent.GetMouseY();
    m_lastX = mmEvent.GetMouseX();
    m_lastY = mmEvent.GetMouseY();

    xOffset *= m_cameraSensitivity;
    yOffset *= m_cameraSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if(m_pitch > 89.0f)
      m_pitch = 89.0f;
    else if(m_pitch < -89.0f)
      m_pitch = -89.0f;

    glm::vec3 dir = glm::vec3(0.0f);
    dir.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    dir.y = sin(glm::radians(m_pitch));
    dir.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forwardVec = dir;
  }
  else
  {
    m_Spectating = false;
    m_firstMouse = true;
  }

  return false;
}
void EditorCamera::OnResize(float width, float height)
{
  m_aspectRatio = width / height;
  RecalculateMatrices();
}

// PRIVATE FUNCTIONS //
void EditorCamera::RecalculateMatrices()
{
  SetProjectionMat(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
  m_viewMat = glm::lookAt(m_cameraPosition, m_cameraPosition + m_forwardVec,
                          glm::vec3(0.0f, 1.0f, 0.0f));
  m_viewProjectionMat = m_Projection * m_viewMat;
}
} // namespace Soda