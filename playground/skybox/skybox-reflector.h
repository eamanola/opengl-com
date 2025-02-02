#ifndef SKYBOX_REFLECTOR_H
#define SKYBOX_REFLECTOR_H

#include "mesh.h"
#include "shader.h"
#include "texture.h"

class SkyboxReflector
{
public:
  SkyboxReflector();
  ~SkyboxReflector() { }

  void draw(const Shader& shader) const;
  void free();

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
