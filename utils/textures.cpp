#include "utils.h"

#include "gl-utils/gl-utils.h"
#include <iostream>
#include <stb_image.h>

GLenum Utils_Textures_adjustFormat(const GLenum format, const unsigned int nrChannels)
{
  bool srgb = (format == GL_SRGB) || (format == GL_SRGB_ALPHA);

  if (nrChannels == 3)
    return srgb ? GL_SRGB : GL_RGB;

  if (nrChannels == 4)
    return srgb ? GL_SRGB_ALPHA : GL_RGBA;

  assert(false);

  return format;
}

unsigned int Utils_Textures_createTexture2D(
  const unsigned char* data,
  const int width,
  const int height,
  const GLenum format,
  const GLenum wrap
)
{
  unsigned int textureId;
  if (data) {
    if (!GLUtils::Textures::createTexture2D(data, width, height, format, wrap, textureId)) {
      std::cout << "Failed to create texture\n";
      Utils::Textures::deleteTexture({ .id = textureId });
      textureId = 0;
    }
  } else {
    std::cout << "Failed to load texture\n";
    textureId = 0;
  }

  return textureId;
}

// TODO: wild guess
GLenum Utils_Textures_imageFormat(TEXTURE_TYPE type)
{
  return type == TEXTURE_TYPE_DIFFUSE ? GL_SRGB_ALPHA : GL_RGBA;
}

unsigned int Utils_Textures_loadTexture2D(const char* path, const GLenum format, const GLenum wrap)
{
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

  const unsigned int textureId = Utils_Textures_createTexture2D(
    data, width, height, Utils_Textures_adjustFormat(format, nrChannels), wrap
  );

  stbi_image_free(data);

  return textureId;
}

unsigned int Utils_Textures_loadTexture2D(
  const unsigned char* buffer, const unsigned int len, const GLenum format, const GLenum wrap
)
{
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data = stbi_load_from_memory(buffer, len, &width, &height, &nrChannels, 0);

  const unsigned int textureId = Utils_Textures_createTexture2D(
    data, width, height, Utils_Textures_adjustFormat(format, nrChannels), wrap
  );

  stbi_image_free(data);

  return textureId;
}

void Utils::Textures::deleteTextures(const std::vector<Texture>& textures)
{
  std::vector<unsigned int> textureIds;
  for (const Texture& t : textures) {
    textureIds.push_back(t.id);
  }

  if (textureIds.size()) {
    if (!GLUtils::Textures::deleteTextures(textureIds.size(), &textureIds[0])) {
      std::cout << "Failed to delete textures\n";
      // for(Texture t : textures)
      // {
      //   std::cout << t.id << ": " << t.key << "\n";
      // }
    }
  }
}

void Utils::Textures::deleteTexture(const Texture& texture)
{
  return Utils::Textures::deleteTextures({ texture });
}

Texture Utils::Textures::loadTexture2D(const char* path, const TEXTURE_TYPE type, const GLenum wrap)
{
  return Texture {
    .id = Utils_Textures_loadTexture2D(path, Utils_Textures_imageFormat(type), wrap),
    .type = type,
  };
}

Texture Utils::Textures::loadTexture2D(
  const unsigned char* buffer, const unsigned int len, const TEXTURE_TYPE type, const GLenum wrap
)
{
  return Texture {
    .id = Utils_Textures_loadTexture2D(buffer, len, Utils_Textures_imageFormat(type), wrap),
    .type = type,
  };
}
