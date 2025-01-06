#ifndef SKYBOX_REFLECTOR
#define SKYBOX_REFLECTOR

#include "mesh.h"
#include "shader.h"
#include "texture.h"

class SkyboxReflector
{
public:
  SkyboxReflector();
  ~SkyboxReflector() { }

  void draw(const Shader& shader);
  void free();

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
