#ifndef UNIFORM_BLOCK_BUFFER
#define UNIFORM_BLOCK_BUFFER

#include <iostream>
#include <glad/gl.h>

class UniformBuffer
{
public:
  UniformBuffer() {}
  ~UniformBuffer() {}

  static bool createBuffer(const std::size_t size, unsigned int& bufferId);
  static bool bindBuffer(const unsigned int bindingId, const unsigned int bufferId);
  static bool update(
    const unsigned int bufferId,
    const unsigned int offset, const std::size_t size, const void* value
  );
  static bool overwrite(const unsigned int bufferId, const std::size_t size, const void* value);
  static bool invalidate(const unsigned int bufferId, const std::size_t size);
  static bool invalidate(const unsigned int bufferId, const unsigned int offset, const std::size_t size);
};
#endif
