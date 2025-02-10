#pragma once

#include "Editor/EditorCamera.h"
#include "Renderer/Camera.h"
#include "Renderer/RendererCamera.h"

#include "Core/Core.h"
#include "Renderer/Texture.h"
#include "Tools/SpriteSheet.h"

namespace Soda
{
// a super static class
// super static means there will only be static functions and no member
// variables
class Renderer2D
{
public:
  struct RendererStats
  {
    uint32_t noOfDrawCalls = 0;
    uint32_t noOfQuads = 0;
    uint32_t noIfTextures = 0;

    // im trying new name "Query"
    // cuz it seems appropriate for this
    const uint32_t QueryNoOfTriangles() { return noOfQuads * 2; }
    const uint32_t QueryNoOfVertices() { return noOfQuads * 4; }
    const uint32_t QueryNoOfIndices() { return noOfQuads * 6; }
  };

  static const RendererStats &GetRendererStats();
  static void ResetRendererStats();

public:
  // Init and Shutdown are like the constructor and destructor of the class
  static void Init();
  static void Shutdown();

  // These command handle the batch rendering stuff and default shader uniform
  // stuff
  static void StartScene(const RendererCamera &camera,
                         const glm::mat4 &transform);
  static void StartScene(const EditorCamera &camera);
  static void StartScene(const OrthoCamera &camera);
  static void StopScene();

  static void Setup(); // this will setup things like vars and stuff
  // this func is partly added because we cant call StartScene when we check
  // batch drawing limit

  // this is the draw call we make
  static void DrawBatch();

  // quads with transforms specified
  static void DrawQuad(const glm::mat4 &transform, const glm::vec4 &color);
  static void DrawQuad(const glm::mat4 &transform,
                       const Ref<Texture2D> &texture, const glm::vec4 &color,
                       float texScale = 1.0f);
  static void DrawQuad(const glm::mat4 &transform,
                       const Ref<SpriteSheetTexture> &spriteSheetTexture,
                       const glm::vec4 &color, float texScale = 1.0f);

  // normal quads that dont rotate
  static void DrawQuad(const glm::vec3 &position, const glm::vec2 &scale,
                       const glm::vec4 &color);
  static void DrawQuad(const glm::vec3 &position, const glm::vec2 &scale,
                       const Ref<Texture2D> &texture, const glm::vec4 &color,
                       float texScale = 1.0f);
  static void DrawQuad(const glm::vec3 &position, const glm::vec2 &scale,
                       const Ref<SpriteSheetTexture> &spriteSheetTexture,
                       const glm::vec4 &color, float texScale = 1.0f);

  // rotation takes aq lot of shit to calculate, so we are making seperate
  // function for rotated quads static void DrawRotatedObject(const
  // Ref<Object2D>& object);
  static void DrawRotatedQuad(const glm::vec3 &position, const float &rotation,
                              const glm::vec2 &scale, const glm::vec4 &color);
  static void DrawRotatedQuad(const glm::vec3 &position, const float &rotation,
                              const glm::vec2 &scale,
                              const Ref<Texture2D> &texture,
                              const glm::vec4 &color, float texScale = 1.0f);
  static void DrawRotatedQuad(const glm::vec3 &position, const float &rotation,
                              const glm::vec2 &scale,
                              const Ref<SpriteSheetTexture> &spriteSheetTexture,
                              const glm::vec4 &color, float texScale = 1.0f);

private:
  void ResetBlendState();
};
} // namespace Soda

// when you make a Renderer3D, dont forget about
// EDIT: i forgot what this was about
// std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformVec3("u_ViewPos",
// m_SceneData->CameraPosition);