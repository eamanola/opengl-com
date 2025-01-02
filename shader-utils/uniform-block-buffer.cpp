#include "uniform-block-buffer.h"

bool UniformBlockBuffer::createBuffer(const std::size_t size, unsigned int& bufferId)
{
  glGenBuffers(1, &bufferId);
  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
  glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  return glGetError() == GL_NO_ERROR;
}

bool UniformBlockBuffer::bindBuffer(const unsigned int bindingId, const unsigned int bufferId)
{
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingId, bufferId);

  return glGetError() == GL_NO_ERROR;
}

bool UniformBlockBuffer::set(
  const unsigned int bufferId,
  std::unordered_map<std::string, unsigned int> offsets,
  const char* uniformName, const std::size_t size, const void* value
)
{
  if(offsets.find(uniformName) != offsets.end())
  {
    const int offset = offsets.at(uniformName);

    glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  return false;
}

bool UniformBlockBuffer::overwrite(const unsigned int bufferId, const std::size_t size, const void* value)
{
  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size, value);
  // glBufferData(GL_UNIFORM_BUFFER, sizeof(padded), &padded[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  return glGetError() == GL_NO_ERROR;
}
