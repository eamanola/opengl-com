#ifndef DRAW_TEXTURE
#define DRAW_TEXTURE

#include "mesh.h"
#include "shader.h"
#include "texture.h"

class DrawTexture
{
public:
  DrawTexture(/* args */);
  ~DrawTexture() { }

  void free() const { mMesh.free(); }

  void render(const Shader& shader, const Texture& texture) const;

private:
  const Mesh mMesh;
};

#endif
