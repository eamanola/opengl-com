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

Texture loadTexture2D(const char* path, TEXTURE_TYPE type);
unsigned int loadTexture2D(const char* path, const int wrap = GL_REPEAT);
unsigned int
loadTexture2D(const unsigned char* buffer, const unsigned int len, const int wrap = GL_REPEAT);
void deleteTextures(const std::vector<Texture>& textures);

unsigned int createTexture2D(
  const int width, const int height, const int nrChannel, const unsigned char* data, const int wrap
);

} // namespace Utils

#endif
