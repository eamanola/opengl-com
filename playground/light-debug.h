#ifndef LIGHT_DEBUG_H
#define LIGHT_DEBUG_H

#include "../mesh.h"
#include "../drawable.h"
#include "../shapes.h"

class LightDebug : public Drawable
{
public:
  LightDebug() : mMesh(Shapes::CUBE_ONLY_P) {}
  ~LightDebug() {}

  void update(const float& time) override {};
  void draw(const Shader& shader) const override { mMesh.draw(shader); }
  virtual void free() const override { mMesh.free(); }


private:
  Mesh mMesh;
};

#endif
