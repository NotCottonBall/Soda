#pragma once

#include "Soda/ECS/SceneCamera.h"
#include "Soda/ECS/ScriptEntity.h"

#include "Soda/Renderer/Texture.h"
#include "Soda/Tools/SpriteSheet.h"

#include "Soda/Renderer/RendererCamera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
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

struct LableComponent
{
  std::string Lable = "NULL";

  LableComponent() = default;
  LableComponent(const LableComponent &) = default;
  LableComponent(const std::string &lable) : Lable(lable) {}
};

struct TransformComponent
{
  glm::vec3 Position = {0.0f, 0.0f, 0.0f};
  glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

  TransformComponent() = default;
  TransformComponent(const TransformComponent &) = default;

  // you know what... component related funcs in component structs makes sence
  // to me
  glm::mat4 GetTransform() const
  {
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3{1.0f, 0.0f, 0.0f}) *
        glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3{0.0f, 1.0f, 0.0f}) *
        glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3{0.0f, 0.0f, 1.0f});

    return glm::mat4(glm::translate(glm::mat4(1.0f), Position) * rotation *
                     glm::scale(glm::mat4(1.0f), Scale));
  }
};

// 2D Components
struct SpriteComponent
{
  glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
  Ref<Texture2D> Texture = nullptr;
  Ref<SpriteSheetTexture> SpriteSheetTexture = nullptr;
  float TextureScale = 1.0f;

  SpriteComponent() = default;
  SpriteComponent(const SpriteComponent &) = default;
  SpriteComponent(const glm::vec4 &color,
                  const Ref<Texture2D> &texture = nullptr)
      : Color(color), Texture(texture)
  {
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
};
} // namespace Soda