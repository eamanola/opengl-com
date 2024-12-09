#include "light.h"

#include "vertex.h"
#include "texture.h"
#include "shader-utils.h"
#include <vector>
#include <iterator>

Light::Light()
{
  setup();
}

Light::~Light()
{
  delete mesh;
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

  std::vector<Texture> textures;

  mesh = new Mesh(vertices, indices, textures);
}

void Light::draw(Shader &shader)
{
  mesh->draw(shader);
}

void Light::free()
{
  mesh->free();
}
