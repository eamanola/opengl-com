#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <glm/glm.hpp>
#include "../mesh.h"

class Window
{
public:
  Window(/* args */);
  ~Window();
  void draw(Shader& shader);
  void free();

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
