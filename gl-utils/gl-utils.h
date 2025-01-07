#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <glad/gl.h>
#include <string>
#include <vector>

namespace GLUtils
{

bool createFramebufferTexture2D(
  const float width,
  const float height,
  unsigned int& outFBO,
  unsigned int& outTextureId,
  unsigned int& outRBO
);
// paths order:
// GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
// GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
// GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
// GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Back
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Front
unsigned int loadCubemap(std::vector<std::string> paths);
bool compileShader(const GLenum type, const char* source, unsigned int& shaderId);
bool createTexture2D(
  const int width,
  const int height,
  const int nrChannel,
  const unsigned char* data,
  unsigned int& textureId,
  const GLint wrap = GL_REPEAT
);
bool deleteTextures(const unsigned int lenght, const unsigned int* textureId);
std::vector<int> checkErrors();
bool noErrors();

} // namespace GLUtils

#endif
