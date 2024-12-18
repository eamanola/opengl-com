#ifndef LIGHT_H
#define LIGHT_H

#include "../mesh.h"

class Light
{
public:
  Light();
  ~Light();

  void draw(Shader &shader);
  void free();

private:
  void setup();
  Mesh* mMesh;
};

#endif
