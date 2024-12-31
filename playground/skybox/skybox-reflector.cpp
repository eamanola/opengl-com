#include "skybox-reflector.h"
#include "../../shader-utils/shader-utils.h"
#include <glm/gtc/matrix_transform.hpp>

SkyboxReflector::SkyboxReflector(/* args */)
:
mMesh(
  std::vector<Vertex> {
    { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, },
    { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, },
    { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.0f,  0.0f, -1.0f }, },

    { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, },
    { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.0f,  0.0f,  1.0f }, },

    { .position { -0.5f, -0.5f, -0.5f }, .normal { -1.0f,  0.0f,  0.0f }, },
    { .position { -0.5f,  0.5f, -0.5f }, .normal { -1.0f,  0.0f,  0.0f }, },
    { .position { -0.5f,  0.5f,  0.5f }, .normal { -1.0f,  0.0f,  0.0f }, },
    { .position { -0.5f, -0.5f,  0.5f }, .normal { -1.0f,  0.0f,  0.0f }, },

    { .position {  0.5f, -0.5f, -0.5f }, .normal {  1.0f,  0.0f,  0.0f }, },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  1.0f,  0.0f,  0.0f }, },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  1.0f,  0.0f,  0.0f }, },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  1.0f,  0.0f,  0.0f }, },

    { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.0f, -1.0f,  0.0f }, },
    { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.0f, -1.0f,  0.0f }, },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.0f, -1.0f,  0.0f }, },
    { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.0f, -1.0f,  0.0f }, },

    { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.0f,  1.0f,  0.0f }, },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.0f,  1.0f,  0.0f }, },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.0f,  1.0f,  0.0f }, },
    { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.0f,  1.0f,  0.0f }, }
  },
  std::vector<unsigned int> {
     0,  2,  1,  0,  3,  2,
     4,  5,  6,  4,  6,  7,
     8, 10,  9,  8, 11, 10,
    12, 13, 14, 12, 14, 15,
    16, 17, 18, 16, 18, 19,
    20, 22, 21, 20, 23, 22
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

void SkyboxReflector::draw(Shader& shader)
{
  shader.setMat4fv("u_model", glm::translate(glm::mat4(1.0), glm::vec3(-1.f, 0.f, -1.f)));
  // reflect ratio
  // Air 	    1.00
  // Water 	  1.33
  // Ice 	    1.309
  // Glass 	  1.52
  // Diamond 	2.42
  shader.setFloat("u_ratio", 1.f / 1.52f);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture.id);
  mMesh.draw(shader);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyboxReflector::free()
{
  mMesh.free();
  ShaderUtils::deleteTextures({ mTexture });
};
