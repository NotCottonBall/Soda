#include "Scene.h"

#include "ECS/ScriptEntity.h"
#include "Renderer/Render.h"
#include "Renderer/RendererCamera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

#include "Renderer/Renderer2D.h"

#include "ECS/Components.h"
#include "ECS/Object.h"

namespace Soda
{
Scene::Scene(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
{
}

Object Scene::CreateObject(const std::string &name)
{
  Object obj = {m_Registry.create(), this};
  obj.AddComponent<NameComponent>(name);
  obj.AddComponent<TagComponent>("NotTagged");
  obj.AddComponent<TransformComponent>();

  return obj;
}
void Scene::DestroyObject(Object obj) { m_Registry.destroy(obj); }

Object Scene::GetPrimaryCamera()
{
  auto view = m_Registry.view<CameraComponent>();
  for(auto obj : view)
  {
    auto camera = view.get<CameraComponent>(obj);
    if(camera.PrimaryCamera)
      return Object{obj, this};
  }
}

// Editor Functions
void Scene::OnEditorUpdate(Timestep dt, EditorCamera &editorCam)
{
  Renderer2D::StartScene(editorCam);
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
void Scene::OnEditorResize(uint32_t width, uint32_t height,
                           EditorCamera &editorcam)
{
  editorcam.OnResize(width, height);
}

// Game Functions
void Scene::OnGameUpdate(Timestep dt)
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
  RendererCamera *SceneCamera = nullptr;
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
void Scene::OnGameResize(uint32_t width, uint32_t height)
{
  m_Width = width;
  m_Height = height;
  auto cameras = m_Registry.view<CameraComponent>();
  for(auto camera : cameras)
  {
    auto &cameraComponent = cameras.get<CameraComponent>(camera);
    if(!cameraComponent.FixedAspectRatio)
      cameraComponent.Camera.SetViewport(width, height);
  }
}

template <typename T> void Scene::OnComponentAdded(Object obj, T &component)
{
  SD_ENGINE_ASSERT(false, "Component Needs To Be Specified While Calling The "
                          "Attachment Setup Function.");
}

template <>
void Scene::OnComponentAdded<NameComponent>(Object obj,
                                            NameComponent &component)
{
}

template <>
void Scene::OnComponentAdded<TagComponent>(Object obj, TagComponent &component)
{
}

template <>
void Scene::OnComponentAdded<TransformComponent>(Object obj,
                                                 TransformComponent &component)
{
}

template <>
void Scene::OnComponentAdded<SpriteComponent>(Object obj,
                                              SpriteComponent &component)
{
}

template <>
void Scene::OnComponentAdded<CameraComponent>(Object obj,
                                              CameraComponent &component)
{
  component.Camera.SetViewport(m_Width, m_Height);
}

template <>
void Scene::OnComponentAdded<ScriptComponent>(Object obj,
                                              ScriptComponent &component)
{
}
} // namespace Soda