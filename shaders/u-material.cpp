#include "u-material.h"
#include "locations.h"

using namespace Lighting;

void u_material::bindTextures(const Shader& shader, const Texture* textures, const unsigned int len)
{
  assert(len <= 4); // add more LOCATION::TEXTURE spots if fail
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  for (unsigned int i = 0; i < len; i++) {
    std::string number;
    std::string name;
    TEXTURE_TYPE type = textures[i].type;
    if (type == TEXTURE_TYPE_DIFFUSE) {
      number = std::to_string(diffuseNr++);
      name = "texture_diffuse";
    } else if (type == TEXTURE_TYPE_SPECULAR) {
      number = std::to_string(specularNr++);
      name = "texture_specular";
    } else if (type == TEXTURE_TYPE_NORMALS) {
      number = std::to_string(normalNr++);
      name = "texture_normal";
    } else if (type == TEXTURE_TYPE_HEIGHT) {
      number = std::to_string(heightNr++);
      name = "texture_height";
    } else {
      assert(false);
    }

    glActiveTexture(GL_TEXTURE0 + LOCATIONS::TEXTURES::MATERIAL0 + i);
    shader.setInt(("u_material." + name + number).c_str(), LOCATIONS::TEXTURES::MATERIAL0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  assert(diffuseNr <= 2);
  assert(specularNr <= 2);
  assert(normalNr <= 2);
  assert(heightNr <= 2);
}

void u_material::bindTexture(const Shader& shader, const Texture* texture)
{
  return u_material::bindTextures(shader, texture, 1);
}

void u_material::unbindTextures(
  const Shader& shader, const Texture* textures, const unsigned int len
)
{
  if (len > 0) {
    for (unsigned int i = 0; i < len; i++) {
      glActiveTexture(GL_TEXTURE0 + LOCATIONS::TEXTURES::MATERIAL0 + i);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);

    // TODO
    shader.setInt("u_material.texture_diffuse1", 0);
    shader.setInt("u_material.texture_specular1", 0);
    shader.setInt("u_material.texture_normal1", 0);
    shader.setInt("u_material.texture_height1", 0);
  }
}

void u_material::unbindTexture(const Shader& shader, const Texture* texture)
{
  return u_material::unbindTextures(shader, texture, 1);
}

void u_material::setColor(const Shader& shader, const PhongColor& color)
{
  shader.setVec4fv("u_material.diffuse_color", color.diffuse);
  shader.setVec4fv("u_material.specular_color", color.specular);
}
