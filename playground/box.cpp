#include "box.h"
#include "../shader-utils.h"

Box::Box()
{
  setup();
}

Box::~Box()
{
  delete mMesh;
}

void Box::setup()
{
  std::vector<Vertex> vertices = {
    { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f, -0.5f, -0.5f }, .normal { -1.0f,  0.0f,  0.0f }, .texCoords { 0.f, 0.f } },
    { .position { -0.5f,  0.5f, -0.5f }, .normal { -1.0f,  0.0f,  0.0f }, .texCoords { 1.f, 0.f } },
    { .position { -0.5f,  0.5f,  0.5f }, .normal { -1.0f,  0.0f,  0.0f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f, -0.5f,  0.5f }, .normal { -1.0f,  0.0f,  0.0f }, .texCoords { 0.f, 1.f } },

    { .position {  0.5f, -0.5f, -0.5f }, .normal {  1.0f,  0.0f,  0.0f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  1.0f,  0.0f,  0.0f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  1.0f,  0.0f,  0.0f }, .texCoords { 1.f, 1.f } },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  1.0f,  0.0f,  0.0f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.0f, -1.0f,  0.0f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.0f, -1.0f,  0.0f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.0f, -1.0f,  0.0f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.0f, -1.0f,  0.0f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.0f,  1.0f,  0.0f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.0f,  1.0f,  0.0f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.0f,  1.0f,  0.0f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.0f,  1.0f,  0.0f }, .texCoords { 0.f, 1.f } }
  };

  std::vector<unsigned int> indices = {
    0,  1,  2,  0,  2,  3,
    4,  5,  6,  4,  6,  7,
    8,  9,  10, 8,  10, 11,
    12, 13, 14, 12, 14, 15,
    16, 17, 18, 16, 18, 19,
    20, 21, 22, 20, 22, 23
  };

  mMesh = new Mesh(vertices, indices);

  mTextures.assign({
    Texture {
      .id = ShaderUtils::loadTexture("assets/container2.png"),
      .type = TEXTURE_TYPE_DIFFUSE,
      .path = "assets/container2.png"
    },
    Texture {
      .id = ShaderUtils::loadTexture("assets/container2_specular.png"),
      .type = TEXTURE_TYPE_SPECULAR,
      .path = "assets/container2_specular.png"
    }
  });
}

void Box::draw(Shader &shader)
{
  mMesh->draw(shader, mTextures);
}

void Box::free()
{
  mMesh->free();

  for(unsigned int i = 0; i < mTextures.size(); i ++)
  {
    glDeleteTextures(1, &i);
  }
}
