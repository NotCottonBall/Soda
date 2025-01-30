#include "Scene.h"

#include "Soda/ECS/ScriptEntity.h"
#include "Soda/Renderer/CameraComponent.h"
#include "Soda/Renderer/Render.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

#include "Soda/Renderer/Renderer2D.h"

#include "Soda/ECS/Components.h"
#include "Soda/ECS/Object.h"
#include <memoryapi.h>

namespace Soda
{
Object Scene::CreateObject(const std::string &name)
{
  Object obj = {m_Registry.create(), this};
  obj.AddComponent<NameComponent>(name);
  obj.AddComponent<TransformComponent>();

  return obj;
}
void Scene::DestroyObject(Object obj) { m_Registry.destroy(obj); }

// Scene Functions
void Scene::OnUpdate(Timestep dt)
{
  // Script Components
  m_Registry.view<ScriptComponent>().each([=](auto entity, auto &script) {
    if(!script.Script)
    {
      script.Script = script.InitScript();
      script.Script->m_Script = Object{entity, this};

      script.Script->OnStart();
    }

    script.Script->OnUpdate(dt);
  });

  // Camera Stuff
  TheCamera *SceneCamera = nullptr;
  glm::mat4 CameraTransform(1.0f);

  auto viewGroup = m_Registry.view<TransformComponent, CameraComponent>();
  for(auto cameras : viewGroup)
  {
    const auto &[Transform, Camera] =
        viewGroup.get<TransformComponent, CameraComponent>(cameras);

    if(Camera.PrimaryCamera)
    {
      SceneCamera = &Camera.Camera;
      CameraTransform = Transform.GetTransform();
      break;
    }
  }

  // Rendering Scene
  if(SceneCamera)
  {
    Renderer2D::StartScene(*SceneCamera, CameraTransform);
    {
      auto group =
          m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
      for(auto entity : group)
      {
        const auto &[Transform, Sprite] =
            group.get<TransformComponent, SpriteComponent>(entity);

        if(Sprite.Texture)
          Renderer2D::DrawQuad(Transform.GetTransform(), Sprite.Texture,
                               Sprite.Color, Sprite.TextureScale);
        else
          Renderer2D::DrawQuad(Transform.GetTransform(), Sprite.Color);
      }
    }
    Renderer2D::StopScene();
  }
}

void Scene::OnResize(uint32_t width, uint32_t height)
{
  auto cameras = m_Registry.view<CameraComponent>();
  for(auto camera : cameras)
  {
    auto &cameraComponent = cameras.get<CameraComponent>(camera);
    if(!cameraComponent.FixedAspectRatio)
      cameraComponent.Camera.SetViewport(width, height);
  }
}
} // namespace Soda