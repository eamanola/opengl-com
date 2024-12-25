#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "shader.h"

class Drawable
{
private:
  /* data */
public:
  Drawable(/* args */) { setModel(glm::mat4(1.f)); }
  virtual ~Drawable() {}
  virtual void setModel(const glm::mat4 model) { mModel = model; };
  virtual const glm::mat4& model() const { return mModel; };
  virtual void update(const float& time) = 0;
  virtual void draw(const Shader& shader) = 0;
  virtual void free() = 0;

protected:
  glm::mat4 mModel;
};

#endif
