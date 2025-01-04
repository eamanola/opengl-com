#include "gl-utils.h"
#include <iostream>
#include <stb_image.h>

unsigned int GLUtils::loadCubemap(std::vector<std::string> paths)
{
  unsigned int textureId;

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  int width, height, nrChannels;
  for(unsigned int i = 0; i < paths.size(); i++) {
    unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
    if(data)
    {
      // no transparency needed, omit format
      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
      );
      stbi_image_free(data);
    }
    else
    {
      std::cout << "cube tex failed to load: " << paths[i] << std::endl;
      glDeleteTextures(1, &textureId);
      stbi_image_free(data);
      textureId = 0;
      break;
    }
  }

  return textureId;
}

bool GLUtils::createTexture2D(
  const int width, const int height, const int nrChannel, const unsigned char* data, unsigned int& textureId, const GLint wrap
)
{
  GLenum format;

  if(nrChannel == 3) format = GL_RGB;
  else if (nrChannel == 4) format = GL_RGBA;

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  return GLUtils::noErrors();
}

bool GLUtils::deleteTextures(const unsigned int lenght, const unsigned int* textureIds)
{
  glDeleteTextures(lenght, textureIds);

  return GLUtils::noErrors();
}
