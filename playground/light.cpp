#include "light.h"
#include "../shader-utils.h"

Light::Light()
:
mMesh(
  std::vector<Vertex> ({
    { .position = {  0.5f,  0.5f,  0.5f } },
    { .position = {  0.5f, -0.5f,  0.5f } },
    { .position = { -0.5f, -0.5f,  0.5f } },
    { .position = { -0.5f,  0.5f,  0.5f } },
    { .position = {  0.5f,  0.5f, -0.5f } },
    { .position = {  0.5f, -0.5f, -0.5f } },
    { .position = { -0.5f, -0.5f, -0.5f } },
    { .position = { -0.5f,  0.5f, -0.5f } }
  }),
  std::vector<unsigned int>({
    0, 1, 2, 0, 2, 3, // front
    4, 5, 1, 4, 1, 0, // right
    7, 6, 5, 7, 5, 4, // back
    3, 2, 6, 3, 6, 7, // left
    4, 0, 3, 4, 3, 7, // top
    1, 5, 6, 1, 6, 2  // bottom
  })
)
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
