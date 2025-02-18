#include "SD_PCH.h"

#include <yaml-cpp/yaml.h>

#include "ECS/Components.h"
#include "ECS/Object.h"
#include "SceneSerializer.h"

namespace Soda
{
YAML::Emitter &operator<<(YAML::Emitter &out, glm::vec3 &v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
  return out;
}
YAML::Emitter &operator<<(YAML::Emitter &out, glm::vec4 &v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

SceneSerializer::SceneSerializer(const Ref<Scene> &scene) : m_scene(scene) {}

static void SerializeEntities(YAML::Emitter &out, Object obj)
{
  out << YAML::BeginMap;
  // @TODO: id of the obj
  out << YAML::Key << "Object" << YAML::Value << "69";
  {
    if(obj.HasComponent<NameComponent>())
    {
      std::string name = obj.GetComponent<NameComponent>().Name;
      out << YAML::Key << "Name" << YAML::Value << name.c_str();
    }
    if(obj.HasComponent<TagComponent>())
    {
      std::string tag = obj.GetComponent<TagComponent>().Tag;
      out << YAML::Key << "Tag" << YAML::Value << tag.c_str();
    }
    if(obj.HasComponent<TransformComponent>())
    {
      auto tran = obj.GetComponent<TransformComponent>();
      out << YAML::Key << "Transform Component";
      out << YAML::BeginMap;
      out << YAML::Key << "Position" << YAML::Value << tran.Position;
      out << YAML::Key << "Rotation" << YAML::Value << tran.Rotation;
      out << YAML::Key << "Scale" << YAML::Value << tran.Scale;
      out << YAML::EndMap;
    }
    if(obj.HasComponent<CameraComponent>())
    {
      out << YAML::Key << "Camera Component" << YAML::BeginMap;
      auto camera = obj.GetComponent<CameraComponent>();
      out << YAML::Key << "Primary Camera" << YAML::Value
          << camera.PrimaryCamera;
      out << YAML::Key << "Fized Astect Ratio" << YAML::Value
          << camera.FixedAspectRatio;
      out << YAML::Key << "Camera Type";
      if(camera.Camera.GetCameraType() == SceneCamera::CameraType::Orthographic)
      {
        out << YAML::Value << "Orthographic";
        out << YAML::Key << "Zoom" << YAML::Value
            << camera.Camera.GetOrthoCameraZoom();
        out << YAML::Key << "Near Plane" << YAML::Value
            << camera.Camera.GetOrthoNearPlane();
        out << YAML::Key << "Far Plane" << YAML::Value
            << camera.Camera.GetOrthoFarPlane();
      }
      else
      {
        out << YAML::Value << "Perspective";
        out << YAML::Key << "FOV" << YAML::Value
            << camera.Camera.GetPerspectiveFov();
        out << YAML::Key << "Near Plane" << YAML::Value
            << camera.Camera.GetPerspectiveNearPlane();
        out << YAML::Key << "Far Plane" << YAML::Value
            << camera.Camera.GetPerspectiveFarPlane();
      }
      out << YAML::EndMap;
    }
    if(obj.HasComponent<SpriteComponent>())
    {
      out << YAML::Key << "Sprite Component" << YAML::BeginMap;
      auto sprite = obj.GetComponent<SpriteComponent>();
      out << YAML::Key << "Color" << YAML::Value << sprite.Color;
      if(sprite.Texture)
      {
        out << YAML::Key << "Texture" << YAML::Value
            << sprite.Texture->GetFilePath();
        out << YAML::Key << "Texture Scale" << YAML::Value
            << sprite.TextureScale;
      }
      out << YAML::EndMap;
    }
  }
  out << YAML::EndMap;
}

void SceneSerializer::Serialize(const std::string &filepath)
{
  std::string ext = std::filesystem::path(filepath).extension().string();
  SD_ENGINE_ASSERT(ext == ".stscn",
                   "You Are Trying To Save A Text Based Scene File With An "
                   "Extension Other Than '.stscn'");
  if(ext != ".stscn")
  {
    SD_ENGINE_ERROR("You Are Trying To Save A Text Based Scene File With An "
                    "Extension Other Than '.stscn'");
    return;
  }

  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "Scene" << YAML::Value << "UnNamed";
  out << YAML::Key << "Objects" << YAML::BeginSeq;

  for(entt::entity entity : m_scene->m_Registry.view<entt::entity>())
  {
    Object obj{entity, m_scene.get()};
    if(!obj)
      return;
    SerializeEntities(out, obj);
  }

  out << YAML::EndSeq << YAML::EndMap;

  std::ofstream file(filepath);
  file << out.c_str();
}
void SceneSerializer::SerializeBinary(const std::string &filepath) {}

void SceneSerializer::Deserialize(const std::string &filepath) {}
void SceneSerializer::DeserializeBinary(const std::string &filepath) {}
} // namespace Soda