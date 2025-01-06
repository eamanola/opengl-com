#ifndef UMATERIAL_H
#define UMATERIAL_H

#include "shader.h"
#include "texture.h"

namespace Lighting
{

class u_material
{

public:
  static void bindTextures(const Shader& shader, const Texture* textures, const unsigned int len)
  {
    assert(len <= 2);
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < len; i++) {
      glActiveTexture(GL_TEXTURE0 + i);

      std::string number;
      std::string name;
      TEXTURE_TYPE type = textures[i].type;
      if (type == TEXTURE_TYPE_DIFFUSE) {
        number = std::to_string(diffuseNr++);
        name = "texture_diffuse";
      } else if (type == TEXTURE_TYPE_SPECULAR) {
        number = std::to_string(specularNr++);
        name = "texture_specular";
      }

      shader.setInt(("u_material." + name + number).c_str(), i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
  }

  static void bindTextures(const Shader& shader, const Texture* textures)
  {
    return bindTextures(shader, textures, 1);
  }

  static void unbindTextures(const Shader& shader, const Texture* textures, const unsigned int len)
  {
    if (len > 0) {
      for (unsigned int i = 0; i < len; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
      glActiveTexture(GL_TEXTURE0);

      // TODO
      shader.setInt("u_material.texture_diffuse1", 0);
      shader.setInt("u_material.texture_specular1", 0);
    }
  }

  static void unbindTextures(const Shader& shader, const Texture* textures)
  {
    return unbindTextures(shader, textures, 1);
  }
};

} // namespace Lighting

#endif
