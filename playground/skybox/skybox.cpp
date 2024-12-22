#include "skybox.h"
#include "../../shader-utils.h"
Skybox::Skybox(/* args */)
:
mMesh(
  std::vector<Vertex> {
    { .position = { -1.0f, -1.0f,  1.0f } },
    { .position = {  1.0f, -1.0f,  1.0f } },
    { .position = {  1.0f,  1.0f,  1.0f } },
    { .position = { -1.0f,  1.0f,  1.0f } },
    { .position = { -1.0f, -1.0f, -1.0f } },
    { .position = {  1.0f, -1.0f, -1.0f } },
    { .position = {  1.0f,  1.0f, -1.0f } },
    { .position = { -1.0f,  1.0f, -1.0f } }
  },
    std::vector<unsigned int> {
      1, 0, 3, 1, 3, 2, // front
      5, 1, 2, 5, 2, 6, // right
      4, 5, 6, 4, 6, 7, // back
      0, 4, 7, 0, 7, 3, // left
      3, 6, 2, 3, 7, 6, // top
      0, 1, 5, 0, 5, 4  // bottom
    }
),
mTexture(
  Texture {
    .id = ShaderUtils::loadCubemap({
      "assets/citadella/negx.jpg",
      "assets/citadella/posx.jpg",
      "assets/citadella/posy.jpg",
      "assets/citadella/negy.jpg",
      "assets/citadella/posz.jpg",
      "assets/citadella/negz.jpg"
      // "assets/skybox/right.jpg",
      // "assets/skybox/left.jpg",
      // "assets/skybox/top.jpg",
      // "assets/skybox/bottom.jpg",
      // "assets/skybox/back.jpg",
      // "assets/skybox/front.jpg"
    }),
    .type = TEXTURE_TYPE_DIFFUSE,
    .path = "assets/citadella/*.jpg"
  }
)
{
}

void Skybox::draw(Shader &shader)
{
  glDepthFunc(GL_LEQUAL);

  glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture.id);
  mMesh.draw(shader);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  glDepthFunc(GL_LESS);
}
