#include "light.h"
#include "../shader-utils.h"
#include "../shapes.h"
Light::Light()
:
mMesh(Shapes::CUBE_ONLY_P)
{
}

Light::~Light()
{
}

void Light::draw(Shader &shader)
{
  mMesh.draw(shader);
}

void Light::free()
{
  mMesh.free();
}
