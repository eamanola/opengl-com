#include "utils.h"

#include <stb_image.h>
#include "gl-utils/gl-utils.h"
#include <iostream>

unsigned int Utils::loadTexture2D(const char* path, const int wrap)
{
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannel;
  unsigned char* data = stbi_load(path, &width, &height, &nrChannel, 0);

  const unsigned int textureId = createTexture2D(width, height, nrChannel, data, wrap);

  stbi_image_free(data);

  return textureId;
}

unsigned int Utils::loadTexture2D(const unsigned char* buffer, const unsigned int len, const int wrap)
{
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannel;
  unsigned char* data = stbi_load_from_memory(buffer, len, &width, &height, &nrChannel, 0);

  const unsigned int textureId = createTexture2D(width, height, nrChannel, data, wrap);

  stbi_image_free(data);

  return textureId;
}

unsigned int Utils::createTexture2D(const int width, const int height, const int nrChannel, const unsigned char* data, const int wrap)
{
  unsigned int textureId;
  if (data) {
    if(!GLUtils::createTexture2D(width, height, nrChannel, data, textureId, wrap))
    {
      std::cout << "Failed to create texture\n";
      deleteTextures({ Texture { .id = textureId } });
      textureId = 0;
    }
  }
  else
  {
    std::cout << "Failed to load texture\n";
    textureId = 0;
  }

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
    if (!GLUtils::deleteTextures(textureIds.size(), &textureIds[0]))
    {
      std::cout << "Failed to delete textures\n";
      // for(Texture t : textures)
      // {
      //   std::cout << t.id << ": " << t.key << "\n";
      // }
    }
  }
}

Texture Utils::loadTexture2D(const char* path, TEXTURE_TYPE type)
{
  return Texture {
    .id = loadTexture2D(path),
    .type = type
  };
}
