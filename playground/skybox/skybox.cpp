#include "skybox.h"
#include "../../shader-utils/shader-utils.h"
#include "../../shapes.h"
Skybox::Skybox(/* args */)
:
mMesh(Shapes::CUBE_ONLY_P),
mTexture(
  Texture {
    .id = ShaderUtils::loadCubemap({
      "assets/skybox/right.jpg",
      "assets/skybox/left.jpg",
      "assets/skybox/bottom.jpg",
      "assets/skybox/top.jpg",
      "assets/skybox/front.jpg",
      "assets/skybox/back.jpg"
    }),
    .type = TEXTURE_TYPE_DIFFUSE,
    .path = "assets/skybox/*.jpg"
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
