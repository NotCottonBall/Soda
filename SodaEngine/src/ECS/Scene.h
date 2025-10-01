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
  Scene(uint32_t width, uint32_t height);
  ~Scene() = default;

  // Object specific code
  Object CreateObject(const std::string &name =
                          "Object"); // Object is basicly entity. I just like to
                                     // call it Object and not Entity
  void DestroyObject(Object obj);

  Object GetObjectWithName(const std::string &name);

  // entity functions
  void OnEditorUpdate(Timestep dt, EditorCamera &editorCam);
  void OnEditorResize(uint32_t width, uint32_t height, EditorCamera &editorcam);
  void OnGameUpdate(Timestep dt);
  void OnGameResize(uint32_t width, uint32_t height);

  Object GetPrimaryCamera();

private:
  template <typename T> void OnComponentAdded(Object obj, T &component);

private:
  entt::registry m_Registry;

  float m_Width = 0, m_Height = 0;

  friend class Object;
  friend class Panels;
  friend class SceneSerializer;
};
} // namespace Soda