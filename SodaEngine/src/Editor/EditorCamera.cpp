#include "SD_PCH.h"

#include "Core/Core.h"

#include "EditorCamera.h"

#include "Input/Input.h"
#include "Input/KeyboardCodes.h"

namespace Soda
{
EditorCamera::EditorCamera(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio),
      m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel,
               -m_ZoomLevel, m_ZoomLevel),
      m_Rotation(rotation)
{
}

void EditorCamera::OnUpdate(float dt)
{
  if(Input::IsKeyPressed(SD_KEY_A))
    m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
  else if(Input::IsKeyPressed(SD_KEY_D))
    m_CameraPosition.x += m_CameraTranslationSpeed * dt;

  if(Input::IsKeyPressed(SD_KEY_W))
    m_CameraPosition.y += m_CameraTranslationSpeed * dt;
  else if(Input::IsKeyPressed(SD_KEY_S))
    m_CameraPosition.y -= m_CameraTranslationSpeed * dt;

  if(m_Rotation)
  {
    if(Input::IsKeyPressed(SD_KEY_Q))
      m_CameraRotation += m_CameraRotationSpeed * dt;
    if(Input::IsKeyPressed(SD_KEY_E))
      m_CameraRotation -= m_CameraRotationSpeed * dt;

    m_Camera.SetRotation(m_CameraRotation);
  }

  m_Camera.SetPosition(m_CameraPosition);
}

void EditorCamera::OnEvent(Event &event)
{
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseScrollEvent>(BIND_FN(EditorCamera::OnMouseScrolled));
  dispatcher.Dispatch<WindowResizeEvent>(
      BIND_FN(EditorCamera::OnWindowResized));
}

bool EditorCamera::OnMouseScrolled(MouseScrollEvent &msEvent)
{
  m_CameraTranslationSpeed = m_ZoomLevel;

  m_ZoomLevel -= msEvent.GetWheelOffsetY() * 0.25f;
  m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

  m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
                         m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                         m_ZoomLevel);

  return false;
}

bool EditorCamera::OnWindowResized(WindowResizeEvent &wrEvent)
{
  WhenResized((float)wrEvent.GetWindowWidth(),
              (float)wrEvent.GetWindowHeight());

  return false;
}

void EditorCamera::WhenResized(float width, float height)
{
  m_AspectRatio = width / height;
  m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
                         m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                         m_ZoomLevel);
}
} // namespace Soda