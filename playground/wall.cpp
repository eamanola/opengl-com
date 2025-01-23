#include "wall.h"
#include "shaders/u-material.h"
#include "shaders/u-model.h"
#include "utils/utils.h"

Wall::Wall(/* args */) :
  mMesh(Shapes::withTangents(Shapes::QUAD)),
  mTextures({
    Utils::Textures::loadTexture2D("./assets/wall/bricks2.jpg", TEXTURE_TYPE_DIFFUSE, true),
    Utils::Textures::loadTexture2D("./assets/wall/bricks2_normal.jpg", TEXTURE_TYPE_NORMALS, false),
    Utils::Textures::loadTexture2D("./assets/wall/bricks2_disp.jpg", TEXTURE_TYPE_HEIGHT, false),
  })
{
}

void Wall::render(const Shader& shader) const
{
  Lighting::u_material::bindTextures(shader, &mTextures[0], mTextures.size());

  u_model::setUModel(shader, model());

  glDisable(GL_CULL_FACE);

  mMesh.draw();

  glEnable(GL_CULL_FACE);

  Lighting::u_material::unbindTextures(shader, &mTextures[0], mTextures.size());
}

void Wall::free() const
{
  mMesh.free();
  Utils::Textures::deleteTextures(mTextures);
}
