#include "Core/Core.h"
#include "SD_PCH.h"


#include "SpriteSheet.h"

namespace Soda
{
SpriteSheetTexture::SpriteSheetTexture(const Ref<Texture2D> &texture,
                                       const glm::vec2 &start,
                                       const glm::vec2 &end)
    // we can imagine start as 0, 0 and end as 1, 1
    : m_Texture(texture)
{
  m_Coords[0] = {start.x, start.y};
  m_Coords[1] = {end.x, start.y};
  m_Coords[2] = {end.x, end.y};
  m_Coords[3] = {start.x, end.y};
}

Ref<SpriteSheetTexture> SpriteSheetTexture::TextureFromSheet(
    const Ref<Texture2D> &texture, const glm::vec2 &coords,
    const glm::vec2 &spriteSize)
{
  // start is like 0, 0 and end is like 1, 1
  glm::vec2 start = {(coords.x * spriteSize.x) / texture->GetWidth(),
                     (coords.y * spriteSize.y) / texture->GetHeight()};
  glm::vec2 end = {
      ((coords.x * spriteSize.x) + spriteSize.x) / texture->GetWidth(),
      ((coords.y * spriteSize.y) + spriteSize.y) / texture->GetHeight()};

  return CreateRef<SpriteSheetTexture>(texture, start, end);
}
} // namespace Soda