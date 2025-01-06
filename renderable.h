#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "shader.h"

class Renderable
{
public:
  Renderable() { setModel(glm::mat4(1.f)); }
  virtual ~Renderable() { }

  virtual void setModel(const glm::mat4 model) { mModel = model; };
  virtual const glm::mat4& model() const { return mModel; };
  virtual void update(const float& time) = 0;
  virtual void render(const Shader& shader) const = 0;
  virtual void free() const = 0;

protected:
  glm::mat4 mModel;
};

#endif
