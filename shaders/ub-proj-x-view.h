#ifndef U_PROJ_X_VIEW_H
#define U_PROJ_X_VIEW_H

#include "gl-utils/uniform-block.h"
#include "gl-utils/uniform-buffer.h"
#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

class ub_proj_x_view
{
public:
  ub_proj_x_view(unsigned int bindingId, const std::vector<Shader>& shaders)
  {
    const char* blockName = "ub_proj_x_view";

    for (Shader shader : shaders) {
      const unsigned int shaderId = shader.id();

      unsigned int blockIndex;
      if (!UniformBlock::getBlockIndex(shaderId, blockName, blockIndex)) {
        std::cout << "Failed to find block\n";
        return;
      }

      if (!UniformBlock::bindBlock(bindingId, shaderId, blockIndex)) {
        std::cout << "Failed to bind shaders \n";
        free();
        return;
      }
    }

    if (!UniformBuffer::createBuffer(sizeof(glm::mat4), mBufferId)) {
      std::cout << "Failed to create buffer\n";
      free();
      return;
    }

    if (!UniformBuffer::bindBuffer(bindingId, mBufferId)) {
      std::cout << "Failed to bind buffer\n";
      free();
      return;
    }
  }
  ~ub_proj_x_view() { }

  void free() const { glDeleteBuffers(1, &mBufferId); }

  void set(const glm::mat4& proj_x_view) const
  {
    UniformBuffer::overwrite(mBufferId, sizeof(glm::mat4), glm::value_ptr(proj_x_view));
  }

private:
  unsigned int mBufferId;
};

#endif
