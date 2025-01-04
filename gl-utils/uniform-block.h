#ifndef UNIFORM_BLOCK_H
#define UNIFORM_BLOCK_H

#include <glad/gl.h>
#include <vector>
#include <string>

class UniformBlock
{
public:
  UniformBlock() {}
  ~UniformBlock() {}

  static bool getBlockIndex(const unsigned int shaderId, const char* blockName, unsigned int& blockIndex);
  static bool getBlockDataSize(const unsigned int shaderId, const unsigned int blockIndex, int& blockDataSize);
  static bool bindBlock(const int bindingId, const unsigned int shaderId, const unsigned int blockIndex);
  static bool getUniformOffsets(
    const unsigned int shaderId, const std::vector<std::string>& uniformNames, int* offsets
  );

private:

};

#endif
