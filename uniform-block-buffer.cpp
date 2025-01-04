#include "uniform-block-buffer.h"
#include "gl-utils/uniform-block.h"
#include "gl-utils/uniform-buffer.h"

UniformBlockBuffer::UniformBlockBuffer(
  const unsigned int bindingId,
  const std::vector<Shader>& shaders,
  const char* blockName,
  std::vector<std::string> uniformNames
)
{
  if(uniformNames.size() == 0)
  {
    std::cout << "no uniforms to bind";
    return;
  }

  if(shaders.size() == 0)
  {
    std::cout << "no shaders to bind";
  }

  const unsigned int shaderId = shaders[0].id();

  std::vector<unsigned int> uniformBlockIndices;
  uniformBlockIndices.reserve(shaders.size());
  for(Shader shader: shaders)
  {
    unsigned int blockIndex;
    if(!UniformBlock::getBlockIndex(shader.id(), blockName, blockIndex))
    {
      std::cout << "Failed to find block\n";
      return;
    }
    uniformBlockIndices.push_back(blockIndex);
  }

  for(unsigned int i = 0; i < shaders.size(); i++)
  {
    if(!UniformBlock::bindBlock(bindingId, shaders[i].id(), uniformBlockIndices[i]))
    {
      std::cout << "Failed to bind shaders \n";
      free();
      return;
    }
  }

  if(!UniformBlock::getBlockDataSize(shaderId, uniformBlockIndices[0], mBlockDataSize))
  {
    std::cout << "Failed to get block data size\n";
    return;
  }

  if(!UniformBuffer::createBuffer(mBlockDataSize, mBufferId))
  {
    std::cout << "Failed to create buffer\n";
    free();
    return;
  }

  if(!UniformBuffer::bindBuffer(bindingId, mBufferId))
  {
    std::cout << "Failed to bind buffer\n";
    free();
    return;
  }

  int offsets[uniformNames.size()];
  if(!UniformBlock::getUniformOffsets(shaderId, uniformNames, offsets))
  {
    std::cout << "Failed to get offsets, error:\n";
    free();
    return;
  }

  mOffsets.reserve(uniformNames.size());
  for(unsigned int i = 0; i < uniformNames.size(); i++)
  {
    mOffsets[uniformNames[i]] = offsets[i];
  }
}

bool UniformBlockBuffer::update(const char* uniformName, const std::size_t size, const void* value) const
{
  if(mOffsets.find(uniformName) != mOffsets.end())
  {
    const unsigned int offset = mOffsets.at(uniformName);

    return UniformBuffer::update(mBufferId, offset, size, value);
  }

  return false;
}

bool UniformBlockBuffer::overwrite(const std::size_t size, const void* value) const
{
  return UniformBuffer::overwrite(mBufferId, size, value);
}
