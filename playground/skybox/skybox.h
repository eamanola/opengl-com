#ifndef SKYBOX_H
#define SKYBOX_H

#include "mesh.h"
#include "shader.h"
#include "texture.h"

class Skybox
{
public:
  Skybox();
  ~Skybox() { }

  void draw(const Shader& shader) const;
  void free();

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
