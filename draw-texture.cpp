#include "draw-texture.h"

DrawTexture::DrawTexture() : mMesh(Shapes::QUAD) { }

void DrawTexture::render(const Texture* textures, const unsigned int len) const
{
  for (unsigned int i = 0; i < len; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  mMesh.draw();

  for (unsigned int i = 0; i < len; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
