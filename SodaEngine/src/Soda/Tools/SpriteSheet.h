#pragma once

#include "Soda/Tools/SpriteSheet.h"
#include "glm/glm.hpp"

#include "Soda/Renderer/Texture.h"

namespace Soda
{
class SpriteSheetTexture
{
public:
  SpriteSheetTexture(const Ref<Texture2D> &texture,
                     const glm::vec2 &textureCoords,
                     const glm::vec2 &spriteSize);
  static Ref<SpriteSheetTexture> TextureFromSheet(const Ref<Texture2D> &texture,
                                                  const glm::vec2 &coords,
                                                  const glm::vec2 &spriteSize);

  const Ref<Texture2D> &GetTexture() { return m_Texture; }
  const glm::vec2 *GetSpriteCoords() { return m_Coords; }

private:
  Ref<Texture2D> m_Texture;

  glm::vec2 m_Coords[4];
  glm::vec2 m_SpriteSize;
};
} // namespace Soda