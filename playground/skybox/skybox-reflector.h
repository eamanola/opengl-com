#ifndef SKYBOX_REFLECTOR
#define SKYBOX_REFLECTOR

#include "../../mesh.h"
#include "../../shader.h"

class SkyboxReflector
{
public:
  SkyboxReflector(/* args */);
  ~SkyboxReflector() { };

  void draw(Shader &shader);
  void free();

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
