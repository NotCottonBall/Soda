#include "SD_PCH.h"

#include <yaml-cpp/yaml.h>

#include "ECS/Components.h"
#include "ECS/Object.h"
#include "SceneSerializer.h"

namespace YAML
{
template <> struct convert<glm::vec3>
{
  static Node encode(const glm::vec3 &v)
  {
    Node node;
    node.push_back(v.x);
    node.push_back(v.y);
    node.push_back(v.z);
    return node;
  };
  static bool decode(const Node &node, glm::vec3 &v)
  {
    if(!node.IsSequence() || node.size() != 3)
      return false;

    v.x = node[0].as<float>();
    v.y = node[1].as<float>();
    v.z = node[2].as<float>();
    return true;
  }
};

template <> struct convert<glm::vec4>
{
  static Node encode(const glm::vec4 &v)
  {
    Node node;
    node.push_back(v.x);
    node.push_back(v.y);
    node.push_back(v.z);
    node.push_back(v.w);
    return node;
  };
  static bool decode(const Node &node, glm::vec4 &v)
  {
    if(!node.IsSequence() || node.size() != 4)
      return false;

    v.x = node[0].as<float>();
    v.y = node[1].as<float>();
    v.z = node[2].as<float>();
    v.w = node[3].as<float>();
    return true;
  }
};
} // namespace YAML

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
  out << YAML::Key << "ObjectID" << YAML::Value
      << std::to_string((uint32_t)obj.GetObjectID());
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
      out << YAML::Key << "TransformComponent";
      out << YAML::BeginMap;
      out << YAML::Key << "Position" << YAML::Value << tran.Position;
      out << YAML::Key << "Rotation" << YAML::Value << tran.Rotation;
      out << YAML::Key << "Scale" << YAML::Value << tran.Scale;
      out << YAML::EndMap;
    }
    if(obj.HasComponent<CameraComponent>())
    {
      out << YAML::Key << "CameraComponent" << YAML::BeginMap;
      auto camera = obj.GetComponent<CameraComponent>();
      out << YAML::Key << "PrimaryCamera" << YAML::Value
          << camera.PrimaryCamera;
      out << YAML::Key << "FixedAspectRatio" << YAML::Value
          << camera.FixedAspectRatio;
      out << YAML::Key << "CameraType";
      if(camera.Camera.GetCameraType() == SceneCamera::CameraType::Orthographic)
        out << YAML::Value << "Orthographic";
      else
        out << YAML::Value << "Perspective";
      out << YAML::Key << "Zoom" << YAML::Value
          << camera.Camera.GetOrthoCameraZoom();
      out << YAML::Key << "OrthoNearPlane" << YAML::Value
          << camera.Camera.GetOrthoNearPlane();
      out << YAML::Key << "OrthoFarPlane" << YAML::Value
          << camera.Camera.GetOrthoFarPlane();
      out << YAML::Key << "FOV" << YAML::Value
          << camera.Camera.GetPerspectiveFov();
      out << YAML::Key << "PersNearPlane" << YAML::Value
          << camera.Camera.GetPerspectiveNearPlane();
      out << YAML::Key << "PersFarPlane" << YAML::Value
          << camera.Camera.GetPerspectiveFarPlane();
      out << YAML::EndMap;
    }
    if(obj.HasComponent<SpriteComponent>())
    {
      out << YAML::Key << "SpriteComponent" << YAML::BeginMap;
      auto sprite = obj.GetComponent<SpriteComponent>();
      out << YAML::Key << "Color" << YAML::Value << sprite.Color;
      if(sprite.Texture)
      {
        out << YAML::Key << "TexturePath" << YAML::Value
            << sprite.Texture->GetFilePath();
        out << YAML::Key << "TextureScale" << YAML::Value
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

void SceneSerializer::Deserialize(const std::string &filepath)
{
  std::string ext = std::filesystem::path(filepath).extension().string();
  SD_ENGINE_ASSERT(ext == ".stscn",
                   "You Are Trying To Load A Text Based Scene File With An "
                   "Extension Other Than '.stscn'");
  if(ext != ".stscn")
  {
    SD_ENGINE_ERROR("You Are Trying To Load A Text Based Scene File With An "
                    "Extension Other Than '.stscn'");
    return;
  }

  YAML::Node data = YAML::LoadFile(filepath);
  SD_ENGINE_ASSERT(data, "Failed To Load: " + filepath);

  if(!data["Scene"])
    SD_ENGINE_LOG("{0} Is Not A Scene File", filepath);

  auto objects = data["Objects"];
  for(auto obj : objects)
  {
    // @TODO: every object needs an ID
    uint64_t uuid = obj["ObjectID"].as<uint64_t>();
    std::string name = obj["Name"].as<std::string>();
    std::string tag = obj["Tag"].as<std::string>();
    Object createdObj = m_scene->CreateObject(name);
    createdObj.GetComponent<TagComponent>().Tag = tag;
    SD_ENGINE_VERBOSE("Deserialized An Object With ID: {0}", uuid);

    if(obj["TransformComponent"])
    {
      auto tComponent = obj["TransformComponent"];
      auto &tc = createdObj.GetComponent<TransformComponent>();
      tc.Position = tComponent["Position"].as<glm::vec3>();
      tc.Rotation = tComponent["Rotation"].as<glm::vec3>();
      tc.Scale = tComponent["Scale"].as<glm::vec3>();
    }
    if(obj["CameraComponent"])
    {
      auto camComponent = obj["CameraComponent"];
      createdObj.AddComponent<CameraComponent>();
      auto &cam = createdObj.GetComponent<CameraComponent>();
      cam.FixedAspectRatio = camComponent["PrimaryCamera"].as<bool>();
      cam.FixedAspectRatio = camComponent["FixedAspectRatio"].as<bool>();
      if(camComponent["CameraType"].as<std::string>() == "Orthographic")
        cam.Camera.SetCameraType(SceneCamera::CameraType::Orthographic);
      else
        cam.Camera.SetCameraType(SceneCamera::CameraType::Perspective);
      // Ortho Values
      cam.Camera.SetOrthoCameraZoom(camComponent["Zoom"].as<float>());
      cam.Camera.SetOrthoNearPlane(camComponent["OrthoNearPlane"].as<float>());
      cam.Camera.SetOrthoFarPlane(camComponent["OrthoFarPlane"].as<float>());
      // Perspective Values
      cam.Camera.SetPerspectiveFov(camComponent["FOV"].as<float>());
      cam.Camera.SetPerspectiveNearPlane(
          camComponent["PersNearPlane"].as<float>());
      cam.Camera.SetPerspectiveFarPlane(
          camComponent["PersFarPlane"].as<float>());
    }
    if(obj["SpriteComponent"])
    {
      auto spComponent = obj["SpriteComponent"];
      createdObj.AddComponent<SpriteComponent>();
      auto &sp = createdObj.GetComponent<SpriteComponent>();
      sp.Color = spComponent["Color"].as<glm::vec4>();
      if(spComponent["Texture"])
      {
        Ref<Texture2D> tex =
            Texture2D::Create(spComponent["TexturePath"].as<std::string>());
        sp.Texture = tex;
        sp.TextureScale = spComponent["TextureScale"].as<float>();
      }
    }
  }
}
void SceneSerializer::DeserializeBinary(const std::string &filepath) {}
} // namespace Soda