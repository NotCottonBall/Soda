#pragma once

#include "Object.h"
#include "Soda/ECS/Scene.h"

#include "Soda/_Main/Timestep.h"

namespace Soda
{
class ScriptEntity
{
public:
  virtual ~ScriptEntity() {}

  template <typename T> T &GetComponent() { m_Script.GetComponent<T>(); }

protected:
  virtual void OnStart() {}
  virtual void OnUpdate(Timestep dt) {}
  virtual void OnDestroy() {}

private:
  Object m_Script;
  friend class Scene;
};
} // namespace Soda