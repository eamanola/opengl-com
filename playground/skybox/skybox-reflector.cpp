#include "skybox-reflector.h"

#include "gl-utils/gl-utils.h"
#include "shaders/locations.h"
#include "shaders/u-model.h"
#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

SkyboxReflector::SkyboxReflector(/* args */) :
  mMesh(
    // clang-format off
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
      { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.0f,  1.0f,  0.0f }, },
    },
    std::vector<unsigned int> {
      0,  2,  1,  0,  3,  2,
      4,  5,  6,  4,  6,  7,
      8, 10,  9,  8, 11, 10,
      12, 13, 14, 12, 14, 15,
      16, 17, 18, 16, 18, 19,
      20, 22, 21, 20, 23, 22,
    } // clang-format on
  ),
  mTexture(Texture {
    .id = GLUtils::Textures::loadCubemap({
      "assets/skybox/right180.jpg",
      "assets/skybox/left180.jpg",
      "assets/skybox/top.jpg",
      "assets/skybox/bottom.jpg",
      "assets/skybox/back180.jpg",
      "assets/skybox/front180.jpg",
    }),
    .type = TEXTURE_TYPE_DIFFUSE,
  })
{
}

void SkyboxReflector::draw(const Shader& shader) const
{
  u_model::setUModel(shader, glm::translate(glm::mat4(1.0), glm::vec3(-3.f, 0.6f, -1.f)));
  // reflect ratio
  // Air 	    1.00
  // Water 	  1.33
  // Ice 	    1.309
  // Glass 	  1.52
  // Diamond 	2.42
  shader.setFloat("u_ratio", 1.f / 1.33f);

  glActiveTexture(GL_TEXTURE0 + LOCATIONS::TEXTURES::SKYBOX_REFLECTOR);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture.id);
  shader.setInt("skybox", LOCATIONS::TEXTURES::SKYBOX_REFLECTOR);

  mMesh.draw();

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyboxReflector::free()
{
  mMesh.free();
  Utils::Textures::deleteTexture(mTexture);
};
