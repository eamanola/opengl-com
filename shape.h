#ifndef SHAPE_H
#define SHAPE_H

#include "shader.h"
#include <glm/glm.hpp>

class Shape : public Shader
{
public:
  Shape(const char* vShader, const char* fShader);
  virtual ~Shape();

  void setModel(glm::mat4 model) const;
  void setView(glm::mat4 view) const;
  void setProjection(glm::mat4 projection) const;
  unsigned int loadTexture(const char * path) const;

  virtual void render() const = 0;
  virtual void free();
};

#endif
