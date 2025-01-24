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
  unsigned int& outFBO,
  unsigned int* outTextureIds,
  unsigned int texturesLen,
  unsigned int* outRBO,
  const unsigned int samples,
  GLenum internal,
  const std::size_t width,
  const std::size_t height,
  GLenum format,
  GLenum type
);

} // namespace Framebuffer

namespace Textures
{

// paths order:
// GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
// GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
// GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
// GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Back
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Front
unsigned int loadCubemap(std::vector<std::string> paths);
bool createTexture2D(
  const unsigned char* data,
  const int width,
  const int height,
  const GLenum format,
  const GLenum wrap,
  unsigned int& textureId
);
bool deleteTextures(const unsigned int length, const unsigned int* textureId);

} // namespace Textures

std::vector<unsigned int> checkErrors();
bool noErrors();

bool compileShader(const GLenum type, const char* source, unsigned int& shaderId);

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
  const void* data,
  const std::size_t size,
  const std::vector<VertexAttribPointer>& attibPointers,
  const BufferUsage usage = BufferUsage::STATIC
);

} // namespace GLUtils

#endif
