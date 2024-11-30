#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shader.h"

class Triangle
{
private:
  Shader* program;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
public:
  Triangle(/* args */);
  ~Triangle();

  bool compile();
  void render();
  void free();
};

#endif
