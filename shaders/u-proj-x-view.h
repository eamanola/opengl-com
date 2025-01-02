#ifndef U_PROJ_X_VIEW_H
#define U_PROJ_X_VIEW_H

#include <glm/gtc/type_ptr.hpp>
#include "../shader-utils/uniform-block-buffer.h"
#include "../shader-utils/uniform-block.h"

class UProjXViewBuffer
{
public:
  UProjXViewBuffer(const std::vector<Shader>& shaders,unsigned int bindingId) {
    const char * blockName = "u_proj_x_view";

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

    if(!UniformBlockBuffer::createBuffer(sizeof(glm::mat4), mBufferId))
    {
      std::cout << "Failed to create buffer\n";
      free();
      return;
    }

    if(!UniformBlockBuffer::bindBuffer(bindingId, mBufferId))
    {
      std::cout << "Failed to bind buffer\n";
      free();
      return;
    }
  }
  ~UProjXViewBuffer() {}

  void free() const { glDeleteBuffers(1, &mBufferId); };

  void set(const glm::mat4& proj_x_view)
  {
    UniformBlockBuffer::overwrite(mBufferId, sizeof(glm::mat4), glm::value_ptr(proj_x_view));
  }

  private:
    unsigned int mBufferId;
    /* data */
};


#endif
