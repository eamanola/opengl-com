#ifndef LIGHT_DEBUG_H
#define LIGHT_DEBUG_H

#include "renderable.h"

#include "mesh.h"
#include "shapes.h"

class LightDebug : public Renderable
{
public:
  LightDebug() : mMesh(Shapes::CUBE_ONLY_P) { }
  ~LightDebug() { }

  void update(const float& time) override { }
  void render(const Shader& shader) const override { mMesh.draw(shader); }
  virtual void free() const override { mMesh.free(); }

private:
  Mesh mMesh;
};

#endif
