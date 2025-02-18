#pragma once

#include "Core/Core.h"
#include "ECS/Object.h"
#include "ECS/Scene.h"

namespace Soda
{
class SceneSerializer
{
public:
  SceneSerializer(const Ref<Scene> &scene);

  void Serialize(const std::string &filepath);
  void SerializeBinary(const std::string &filepath);

  void Deserialize(const std::string &filepath);
  void DeserializeBinary(const std::string &filepath);

private:
  Ref<Scene> m_scene;
};
} // namespace Soda