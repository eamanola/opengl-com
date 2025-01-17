#include "shadows-debug.h"

ShadowsDebug::ShadowsDebug() : mMesh(Shapes::QUAD) { }

ShadowsDebug::~ShadowsDebug() { }

void ShadowsDebug::render(const Shader& shader) const
{
  shader.setMat4fv("u_model", model());

  mMesh.draw();
}

void ShadowsDebug::free() const { mMesh.free(); }
