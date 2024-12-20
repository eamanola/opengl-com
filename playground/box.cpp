#include "box.h"
#include "../shader-utils.h"
#include "glm/gtc/matrix_transform.hpp"

Box::Box()
:
mPositions({
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
})
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
  for(unsigned int i = 0; i < mPositions.size(); i++)
  {
    glm::mat4 model = glm::translate(glm::mat4(1.0), mPositions[i]);
    model = glm::rotate(model, glm::radians(20.f  * i), glm::vec3(1.0f, 0.3f, 0.5f));
    shader.setMat4fv("u_model", model);
    mMesh->draw(shader, mTextures);

    // if(i == boxPositions.size() - 1)
    // {
    //   highlight(box, boxModel);
    //   lightingProgram.use();
    // } else {
    //
    //   box.draw(lightingProgram);
    // }
  }


}

void Box::free()
{
  mMesh->free();

  for(unsigned int i = 0; i < mTextures.size(); i ++)
  {
    glDeleteTextures(1, &i);
  }
}
