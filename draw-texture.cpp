#include "draw-texture.h"

DrawTexture::DrawTexture() : mMesh(Shapes::QUAD) { }

void DrawTexture::render(const Shader& shader, const Texture& texture) const
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  mMesh.draw();
  glBindTexture(GL_TEXTURE_2D, 0);
}
