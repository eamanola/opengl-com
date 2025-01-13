#include "gl-utils.h"
#include <iostream>
#include <stb_image.h>

unsigned int GLUtils::Textures::loadCubemap(std::vector<std::string> paths)
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
  for (unsigned int i = 0; i < paths.size(); i++) {
    unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      // no transparency needed, omit format
      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0,
        GL_SRGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data
      );
      stbi_image_free(data);
    } else {
      std::cout << "cube tex failed to load: " << paths[i] << std::endl;
      glDeleteTextures(1, &textureId);
      stbi_image_free(data);
      textureId = 0;
      break;
    }
  }

  return textureId;
}

bool GLUtils::Textures::createTexture2D(
  const unsigned char* data,
  const int width,
  const int height,
  GLenum internalFormat,
  const GLenum wrap,
  unsigned int& textureId
)
{
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  GLenum format;
  if (internalFormat == GL_SRGB_ALPHA)
    format = GL_RGBA;
  else if (internalFormat == GL_SRGB)
    format = GL_RGB;
  else
    format = internalFormat;

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  return GLUtils::noErrors();
}

bool GLUtils::Textures::deleteTextures(const unsigned int length, const unsigned int* textureIds)
{
  glDeleteTextures(length, textureIds);

  return GLUtils::noErrors();
}
