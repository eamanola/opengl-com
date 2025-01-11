#include "gl-utils/gl-utils.h"

using namespace GLUtils;

bool GLUtils::addVertexBuffer(
  unsigned int& bufferId,
  const unsigned int VAO,
  const void* data,
  const std::size_t size,
  const std::vector<VertexAttribPointer>& attibPointers,
  const BufferUsage usage
)
{
  glBindVertexArray(VAO);
  glGenBuffers(1, &bufferId);
  glBindBuffer(GL_ARRAY_BUFFER, bufferId);

  glBufferData(GL_ARRAY_BUFFER, size, data, usage);

  for (VertexAttribPointer v : attibPointers) {
    glVertexAttribPointer(v.location, v.size, v.type, v.normalized, v.stride, v.offset);

    if (v.divisor > 0) {
      glVertexAttribDivisor(v.location, v.divisor);
    }

    glEnableVertexAttribArray(v.location);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0); // Saved to VAO with glVertexAttribPointer
  glBindVertexArray(0);

  if (GLUtils::noErrors()) {
    return true;
  } else {
    glDeleteBuffers(1, &bufferId);
    bufferId = 0;
    return false;
  }
}
