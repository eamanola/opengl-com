#include "skybox.h"

#include "gl-utils/gl-utils.h"
#include "shapes.h"
#include "utils/utils.h"

Skybox::Skybox() :
  mMesh(Shapes::CUBE_ONLY_P),
  mTexture(Texture {
    .id = GLUtils::Textures::loadCubemap({
      "assets/skybox/right.jpg",
      "assets/skybox/left.jpg",
      "assets/skybox/bottom.jpg",
      "assets/skybox/top.jpg",
      "assets/skybox/front.jpg",
      "assets/skybox/back.jpg",
    }),
    .type = TEXTURE_TYPE_DIFFUSE,
  })
{
}

void Skybox::draw(const Shader& shader) const
{
  glDepthFunc(GL_LEQUAL);

  glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture.id);

  mMesh.draw();

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  glDepthFunc(GL_LESS);
}

void Skybox::free()
{
  mMesh.free();
  Utils::Textures::deleteTexture(mTexture);
}
