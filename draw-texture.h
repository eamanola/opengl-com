#ifndef DRAW_TEXTURE
#define DRAW_TEXTURE

#include "mesh.h"
#include "texture.h"

class DrawTexture
{
public:
  DrawTexture(/* args */);
  ~DrawTexture() { }

  void free() const { mMesh.free(); }

  void render(const Texture* texture, const unsigned int len = 1) const;

private:
  const Mesh mMesh;
};

#endif
