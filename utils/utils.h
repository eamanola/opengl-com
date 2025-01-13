#ifndef UTILS_H
#define UTILS_H

#include "scene.h"
#include "texture.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Utils
{

void screenshot(
  Scene& scene, const unsigned& FBO, const glm::vec3& position, const glm::vec3& normal
);

std::string readFile(const char* path);

std::string shaderSource(
  const char* path,
  const std::vector<std::string>& prependTexts,
  const std::vector<std::string>& prependFiles
);

namespace Textures
{

Texture loadTexture2D(const char* path, const TEXTURE_TYPE type, const GLenum wrap = GL_REPEAT);
Texture loadTexture2D(
  const unsigned char* buffer,
  const unsigned int len,
  const TEXTURE_TYPE type,
  const GLenum wrap = GL_REPEAT
);
void deleteTextures(const std::vector<Texture>& textures);
void deleteTexture(const Texture& texture);

} // namespace Textures

} // namespace Utils

#endif
