#include "skybox.h"

#include "gl-utils/gl-utils.h"
#include "shaders/locations.h"
#include "shapes.h"
#include "utils/utils.h"

Skybox::Skybox() :
  mMesh(Shapes::SKYBOX),
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

void Skybox::draw(const Shader& shader) const
{
  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LEQUAL);

  glActiveTexture(GL_TEXTURE0 + LOCATIONS::TEXTURES::SKYBOX_REFLECTOR);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture.id);
  shader.setInt("skybox", LOCATIONS::TEXTURES::SKYBOX_REFLECTOR);
  mMesh.draw();

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  // glCullFace(GL_BACK);
  // glFrontFace(GL_CCW);

  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
}

void Skybox::free()
{
  mMesh.free();
  Utils::Textures::deleteTexture(mTexture);
}
