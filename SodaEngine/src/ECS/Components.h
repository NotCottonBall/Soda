#pragma once

#include "ECS/SceneCamera.h"
#include "ECS/ScriptEntity.h"

#include "Renderer/Texture.h"
#include "Tools/SpriteSheet.h"

#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/trigonometric.hpp"

namespace Soda
{
// Global Components
struct NameComponent
{
  std::string Name = "Object";

  NameComponent() = default;
  NameComponent(const NameComponent &) = default;
  NameComponent(const std::string &name) : Name(name) {}
};

struct TagComponent
{
  std::string Tag = "None";

  TagComponent() = default;
  TagComponent(const TagComponent &) = default;
  TagComponent(const std::string &tag) : Tag(tag) {}
};

struct TransformComponent
{
  glm::vec3 Position = {0.0f, 0.0f, 0.0f};
#ifndef SD_ExportBuild
  glm::vec3 EulerAngles = {0.0f, 0.0f, 0.0f};
#endif
  glm::quat Rotation = glm::quat(glm::vec3(0.0f));
  glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

  TransformComponent() = default;
  TransformComponent(const TransformComponent &) = default;

  glm::mat4 GetTransform() const
  {
    glm::mat4 rotation = glm::toMat4(Rotation);
    return glm::translate(glm::mat4(1.0f), Position) * rotation *
           glm::scale(glm::mat4(1.0f), Scale);
  }

  // Basis Of The Matrix
  glm::mat3 GetBasis() const { return glm::mat3_cast(Rotation); }
  glm::vec3 GetUp() const { return GetBasis()[1]; }
  glm::vec3 GetForward() const { return -GetBasis()[2]; }
  glm::vec3 GetRight() const { return GetBasis()[0]; }

  // Rotation Functions
  void RotateGlobal(const glm::vec3 &rotation)
  {
    Rotation = glm::quat(glm::radians(rotation));
  }
  void RotateLocal(const glm::vec3 &axis, float angle)
  {
    glm::quat rot = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
    Rotation = rot * Rotation;
#ifndef SD_EditorBuild
    float yaw, pitch, roll;
    glm::extractEulerAngleYXZ(glm::toMat4(Rotation), yaw, pitch, roll);
    EulerAngles = glm::degrees(glm::vec3(pitch, yaw, roll));
#endif
  }
  void RotateYaw(float angle) { RotateLocal(GetUp(), angle); }
  void RotatePitch(float angle) { RotateLocal(GetRight(), angle); }
  void RotateRoll(float angle) { RotateLocal(GetForward(), angle); }

  void Reset()
  {
    Position = {0.0f, 0.0f, 0.0f};
    EulerAngles = {0.0f, 0.0f, 0.0f};
    Rotation = glm::quat(glm::vec3(0.0f));
    Scale = {1.0f, 1.0f, 1.0f};
  }
};

// 2D Components
struct SpriteComponent
{
  glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
  Ref<Texture2D> Texture = nullptr;
  // @TODO: sprite sheet should be a seperate component
  Ref<SpriteSheetTexture> SpriteSheetTexture = nullptr;
  float TextureScale = 1.0f;

  SpriteComponent() = default;
  SpriteComponent(const SpriteComponent &) = default;
  SpriteComponent(const glm::vec4 &color,
                  const Ref<Texture2D> &texture = nullptr)
      : Color(color), Texture(texture)
  {
  }

  void Reset()
  {
    Color = {1.0f, 1.0f, 1.0f, 1.0f};
    TextureScale = 1.0f;
    Texture = nullptr;
    SpriteSheetTexture = nullptr;
  }
};

// Other Components
struct CameraComponent
{
  SceneCamera Camera;
  bool PrimaryCamera = true;
  bool FixedAspectRatio = false;

  CameraComponent() = default;
  CameraComponent(const CameraComponent &) = default;

  void Reset()
  {
    PrimaryCamera = true;
    FixedAspectRatio = false;
  }
};

struct ScriptComponent
{
  ScriptEntity *Script = nullptr;

  ScriptEntity *(*InitScript)();
  void (*DestroyScript)(ScriptComponent *);

  template <typename T> void Bind()
  {
    InitScript = []() { return static_cast<ScriptEntity *>(new T()); };

    DestroyScript = [](ScriptComponent *script) {
      delete script->Script;
      script->Script = nullptr;
    };
  }

  void Reset() { Script = nullptr; }
};
} // namespace Soda