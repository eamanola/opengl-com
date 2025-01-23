#include "toy.h"
#include "shaders/u-material.h"
#include "shaders/u-model.h"
#include "utils/utils.h"

Toy::Toy(/* args */) :
  mMesh(Shapes::withTangents(Shapes::QUAD)),
  mTextures({
    Utils::Textures::loadTexture2D("./assets/toy/wood.png", TEXTURE_TYPE_DIFFUSE, true),
    Utils::Textures::loadTexture2D("./assets/toy/toy_box_normal.png", TEXTURE_TYPE_NORMALS, false),
    Utils::Textures::loadTexture2D("./assets/toy/toy_box_disp.png", TEXTURE_TYPE_HEIGHT, false),
  })
{
}

void Toy::render(const Shader& shader) const
{
  Lighting::u_material::bindTextures(shader, &mTextures[0], mTextures.size());

  u_model::setUModel(shader, model());

  glDisable(GL_CULL_FACE);

  mMesh.draw();

  glEnable(GL_CULL_FACE);

  Lighting::u_material::unbindTextures(shader, &mTextures[0], mTextures.size());
}

void Toy::free() const
{
  mMesh.free();
  Utils::Textures::deleteTextures(mTextures);
}
