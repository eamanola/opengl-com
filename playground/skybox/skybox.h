#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../mesh.h"
#include "../../shader.h"

class Skybox
{
public:
  Skybox(/* args */);
  ~Skybox() { };

  void draw(Shader &shader);
  void free() {
    mMesh.free();
    glDeleteTextures(1, &mTexture.id);
  }
private:
  Mesh mMesh;
  Texture mTexture;
};


#endif
