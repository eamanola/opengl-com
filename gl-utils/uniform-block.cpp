#include "uniform-block.h"
#include "gl-utils.h"

bool UniformBlock::getBlockIndex(
  const unsigned int shaderId, const char* blockName, unsigned int& blockIndex
)
{
  blockIndex = glGetUniformBlockIndex(shaderId, blockName);

  return GLUtils::noErrors();
}

bool UniformBlock::getBlockDataSize(
  const unsigned int shaderId, const unsigned int blockIndex, int& blockDataSize
)
{
  glGetActiveUniformBlockiv(
    shaderId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockDataSize
  );

  return GLUtils::noErrors();
}

bool UniformBlock::bindBlock(const int bindingId, const unsigned int shaderId, const unsigned int blockIndex)
{
  glUniformBlockBinding(shaderId, blockIndex, bindingId);

  return GLUtils::noErrors();
}

bool UniformBlock::getUniformOffsets(
  const unsigned int shaderId, const std::vector<std::string>& uniformNames, int* offsets
)
{
  const unsigned int uniformCount = uniformNames.size();

  std::vector<const char *> names;
  names.reserve(uniformCount);
  for(unsigned int i = 0; i < uniformCount; i++)
  {
    names.push_back(uniformNames[i].c_str());
  }

  unsigned int indices[uniformCount];
  glGetUniformIndices(shaderId, uniformCount, &names[0], indices);

  glGetActiveUniformsiv(shaderId, uniformCount, indices, GL_UNIFORM_OFFSET, offsets);

  return GLUtils::noErrors();
}
