#ifndef SHAPE_H
#define SHAPE_H

#include "shader.h"
#include <glm/glm.hpp>

class Shape
{
public:
  Shape(const char* vShader, const char* fShader);
  virtual ~Shape();

  void setModel(glm::mat4 model);
  void setView(glm::mat4 view);
  void setProjection(glm::mat4 projection);
  unsigned int loadTexture(const char * path);
  Shader program;

  virtual void render() = 0;
  virtual void free();
};

#endif
