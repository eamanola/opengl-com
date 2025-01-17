#ifndef SHADOWS_DEBUG_H
#define SHADOWS_DEBUG_H

#include "mesh.h"
#include "renderable.h"
#include "texture.h"

class ShadowsDebug : public Renderable
{
private:
  /* data */
public:
  ShadowsDebug();
  ~ShadowsDebug();

  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

  Mesh mMesh;
};

#endif
