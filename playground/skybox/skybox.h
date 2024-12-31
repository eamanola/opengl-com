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
  void free();

private:
  Mesh mMesh;
  Texture mTexture;
};


#endif
