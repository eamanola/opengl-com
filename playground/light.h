#ifndef LIGHT_H
#define LIGHT_H

#include "../mesh.h"
#include "../drawable.h"
#include "../shapes.h"

class Light : public Drawable
{
public:
  Light() : mMesh(Shapes::CUBE_ONLY_P) {}
  ~Light() {}

  void update(const float& time) override {};
  void draw(const Shader& shader) override { mMesh.draw(shader); }
  virtual void free() override { mMesh.free(); }


private:
  Mesh mMesh;
};

#endif
