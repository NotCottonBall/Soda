#pragma once

#include "entt.hpp"
#include "glm/glm.hpp"

#include "Core/Timestep.h"
#include "Editor/EditorCamera.h"

namespace Soda
{
class Object;

class Scene
{
public:
  Scene() = default;
  ~Scene() = default;

  // Object specific code
  Object CreateObject(const std::string &name =
                          "Object"); // Object is basicly entity. I just like to
                                     // call it Object and not Entity
  void DestroyObject(Object obj);

  // entity functions
  void OnEditorUpdate(Timestep dt, EditorCamera &editorCam);
  void OnEditorResize(uint32_t width, uint32_t height, EditorCamera &editorcam);
  void OnGameUpdate(Timestep dt);
  void OnGameResize(uint32_t width, uint32_t height);

  Object GetPrimaryCamera();

private:
  entt::registry m_Registry;

  friend class Object;
  friend class Panels;
};
} // namespace Soda