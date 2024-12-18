#include "light.h"
#include "../shader-utils.h"

Light::Light()
{
  setup();
}

Light::~Light()
{
  delete mMesh;
}

void Light::setup()
{
  std::vector<Vertex> vertices = {
    { .position = {  0.5f,  0.5f,  0.5f } },
    { .position = {  0.5f, -0.5f,  0.5f } },
    { .position = { -0.5f, -0.5f,  0.5f } },
    { .position = { -0.5f,  0.5f,  0.5f } },
    { .position = {  0.5f,  0.5f, -0.5f } },
    { .position = {  0.5f, -0.5f, -0.5f } },
    { .position = { -0.5f, -0.5f, -0.5f } },
    { .position = { -0.5f,  0.5f, -0.5f } }
  };

  std::vector<unsigned int> indices = {
    0, 1, 2, 0, 2, 3, // front
    4, 5, 1, 4, 1, 0, // right
    7, 6, 5, 7, 5, 4, // back
    3, 2, 6, 3, 6, 7, // left
    4, 0, 3, 4, 3, 7, // top
    1, 5, 6, 1, 6, 2  // bottom
  };

  mMesh = new Mesh(vertices, indices);
}

void Light::draw(Shader &shader)
{
  std::vector<Texture> textures;
  mMesh->draw(shader, textures);
}

void Light::free()
{
  mMesh->free();
}
