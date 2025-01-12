#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <glad/gl.h>
#include <string>
#include <vector>

namespace GLUtils
{

namespace Framebuffer
{

bool createFramebufferTexture2D(
  const float width,
  const float height,
  const unsigned int samples,
  unsigned int& outFBO,
  unsigned int& outTextureId,
  unsigned int* outRBO
);

bool addTexture(
  const float width, const float height, const unsigned int samples, unsigned int& textureId
);
bool addRenderBuffer(
  const float width, const float height, const unsigned int samples, unsigned int& outRBO
);

} // namespace Framebuffer
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

enum AttribType { FLOAT = GL_FLOAT, UNSIGNED_INT = GL_UNSIGNED_INT };
enum BufferUsage { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW };
struct VertexAttribPointer {
  unsigned int location;
  std::size_t size;
  AttribType type = AttribType::FLOAT;
  GLboolean normalized = GL_FALSE;
  std::size_t stride;
  void* offset;
  unsigned int divisor = 0;
};

bool addVertexBuffer(
  unsigned int& bufferId,
  const unsigned int VAO,
  const void* data,
  const std::size_t size,
  const std::vector<VertexAttribPointer>& attibPointers,
  const BufferUsage usage = BufferUsage::STATIC
);

} // namespace GLUtils

#endif
