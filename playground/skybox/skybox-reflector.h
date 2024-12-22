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
  void free() {
    mMesh.free();
    glDeleteTextures(1, &mTexture.id);
  };

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
