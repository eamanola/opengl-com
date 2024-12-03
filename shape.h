#ifndef SHAPE_H
#define SHAPE_H

#include "shader.h"
#include <glm/glm.hpp>

class Shape
{
public:
  Shape(const char* vShader, const char* fShader);
  virtual ~Shape();

  virtual void free();
  void setModel(glm::mat4 model);
  void setView(glm::mat4 view);
  void setProjection(glm::mat4 projection);
  Shader program;

  virtual void render() = 0;
};

#endif
