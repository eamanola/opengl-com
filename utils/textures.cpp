#include "utils.h"

#include <stb_image.h>
#include "gl-utils/gl-utils.h"
#include <iostream>

unsigned int Utils::loadTexture2D(const char* path, const int wrap)
{
  unsigned int textureId = 0;

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannel;
  unsigned char* data = stbi_load(path, &width, &height, &nrChannel, 0);
  if (data) {
    textureId = GLUtils::createTexture2D(width, height, nrChannel, data, wrap);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  return textureId;
}

unsigned int Utils::loadTexture2D(const unsigned char* buffer, const unsigned int len, const int wrap)
{
  unsigned int textureId = 0;

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannel;
  unsigned char* data = stbi_load_from_memory(buffer, len, &width, &height, &nrChannel, 0);
  if (data) {
    textureId = GLUtils::createTexture2D(width, height, nrChannel, data, wrap);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  return textureId;
}

void Utils::deleteTextures(const std::vector<Texture>& textures)
{
  std::vector<unsigned int> textureIds;
  for(const Texture& t: textures)
  {
    textureIds.push_back(t.id);
  }

  if(textureIds.size())
  {
    GLUtils::deleteTextures(textureIds.size(), &textureIds[0]);
  }
}

Texture Utils::loadTexture2D(const char* path, TEXTURE_TYPE type)
{
  return Texture {
    .id = loadTexture2D(path),
    .type = type
  };
}
