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
  glm::vec3 EulerRotation = {0.0f, 0.0f, 0.0f};    // UI-facing rotation
  glm::quat Rotation = glm::quat(glm::vec3(0.0f)); // math-facing rotation
  glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

  TransformComponent() = default;
  TransformComponent(const TransformComponent &) = default;

  glm::mat4 GetTransform() const
  {
    glm::mat4 rotation = glm::toMat4(Rotation);
    return glm::translate(glm::mat4(1.0f), Position) * rotation *
           glm::scale(glm::mat4(1.0f), Scale);
  }

  void ApplyRotationDelta(const glm::vec3 &deltaEuler)
  {
    // Build incremental quaternions for each axis
    glm::quat qx =
        glm::angleAxis(glm::radians(deltaEuler.x), glm::vec3(1, 0, 0));
    glm::quat qy =
        glm::angleAxis(glm::radians(deltaEuler.y), glm::vec3(0, 1, 0));
    glm::quat qz =
        glm::angleAxis(glm::radians(deltaEuler.z), glm::vec3(0, 0, 1));

    // Multiply onto the existing rotation (local space)
    Rotation = Rotation * qx * qy * qz;
  }

  inline glm::vec3 GetRotationEuler() const { return EulerRotation; }

  // Call this whenever EulerRotation is changed
  void UpdateRotation() { Rotation = glm::quat(glm::radians(EulerRotation)); }

  void Reset()
  {
    Position = {0.0f, 0.0f, 0.0f};
    EulerRotation = {0.0f, 0.0f, 0.0f};
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