#include "uniform-buffer.h"

bool UniformBuffer::createBuffer(const std::size_t size, unsigned int& bufferId)
{
  glGenBuffers(1, &bufferId);
  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
  glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  return glGetError() == GL_NO_ERROR;
}

bool UniformBuffer::bindBuffer(const unsigned int bindingId, const unsigned int bufferId)
{
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingId, bufferId);

  return glGetError() == GL_NO_ERROR;
}

bool UniformBuffer::update(
  const unsigned int bufferId, const unsigned int offset, const std::size_t size, const void* value
)
{
  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  return glGetError() == GL_NO_ERROR;
}

bool UniformBuffer::overwrite(const unsigned int bufferId, const std::size_t size, const void* value)
{
  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
  glBufferData(GL_UNIFORM_BUFFER, size, value, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  return glGetError() == GL_NO_ERROR;
}

bool UniformBuffer::invalidate(const unsigned int bufferId, const unsigned int offset, const std::size_t size)
{
  glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
  glMapBufferRange(GL_UNIFORM_BUFFER, offset, size, GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT);
  glUnmapBuffer(GL_UNIFORM_BUFFER);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  return glGetError() == GL_NO_ERROR;
}

bool UniformBuffer::invalidate(const unsigned int bufferId, const std::size_t size)
{
  return UniformBuffer::overwrite(bufferId, size, NULL);
}
