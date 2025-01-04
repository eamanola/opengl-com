#ifndef UNIFORM_BLOCK_BUFFER_H
#define UNIFORM_BLOCK_BUFFER_H

#include <vector>
#include "shader.h"
#include <unordered_map>

class UniformBlockBuffer
{
public:
  UniformBlockBuffer(
    const unsigned int bindingId,
    const std::vector<Shader>& shaders,
    const char* blockName,
    std::vector<std::string> uniformNames
  );
  ~UniformBlockBuffer() {};

  void free() const { glDeleteBuffers(1, &mBufferId); }
  bool update(const char* uniformName, const std::size_t size, const void* value) const;
  bool overwrite(const std::size_t size, const void* value) const;

protected:
  std::unordered_map<std::string, unsigned int> mOffsets;
  int mBlockDataSize;

private:
  unsigned int mBufferId;
};

#endif
