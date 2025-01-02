#ifndef UNIFORM_BLOCK_BUFFER
#define UNIFORM_BLOCK_BUFFER

#include <iostream>
#include <glad/gl.h>
#include <unordered_map>

class UniformBlockBuffer
{
public:
  UniformBlockBuffer() {}
  ~UniformBlockBuffer() {}

  static bool createBuffer(const std::size_t size, unsigned int& bufferId);
  static bool bindBuffer(const unsigned int bindingId, const unsigned int bufferId);
  static bool set(
    const unsigned int bufferId,
    std::unordered_map<std::string, unsigned int> offsets,
    const char* uniformName, const std::size_t size, const void* value
  );
  static bool overwrite(const unsigned int bufferId, const std::size_t size, const void* value);
};
#endif
