#pragma once

#include "ECS/Scene.h"
#include "Object.h"


#include "Core/Timestep.h"

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