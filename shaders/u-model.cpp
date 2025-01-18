#include "u-model.h"

#include "gl-utils/gl-utils.h"
#include "shaders/locations.h"
#include <iostream>

unsigned int u_model::addModelsBuffer(const std::vector<glm::mat4>& models)
{
  const std::size_t stride = sizeof(std::pair<glm::mat4, glm::mat3>);
  const std::size_t sizeOfVec3 = sizeof(glm::vec3);
  const std::size_t sizeOfVec4 = sizeof(glm::vec4);
  const std::size_t sizeOfMat4 = sizeof(glm::mat4);

  std::vector<GLUtils::VertexAttribPointer> modelAttribs;
  for (unsigned int i = 0; i < 4; i++) {
    modelAttribs.push_back(GLUtils::VertexAttribPointer {
      .location = LOCATIONS::ATTRIBS::INSTANCED_MODELS + i,
      .size = 4,
      .stride = stride,
      .offset = (void*)(i * sizeOfVec4),
      .divisor = 1,
    });
  }
  for (unsigned int i = 0; i < 3; i++) {
    modelAttribs.push_back(GLUtils::VertexAttribPointer {
      .location = LOCATIONS::ATTRIBS::INSTANCED_TRA_INV_MODELS + i,
      .size = 3,
      .stride = stride,
      .offset = (void*)(sizeOfMat4 + i * sizeOfVec3),
      .divisor = 1,
    });
  }

  std::vector<std::pair<glm::mat4, glm::mat3>> data;
  for (unsigned int i = 0; i < models.size(); i++) {
    data.push_back({ models[i], glm::mat3(glm::transpose(glm::inverse(models[i]))) });
  }

  unsigned int vbo;

  if (!GLUtils::addVertexBuffer(vbo, &data[0], stride * data.size(), modelAttribs)) {
    std::cout << "failed to create models buffer\n";
    vbo = 0;
  }

  return vbo;
}

void u_model::setUModel(const Shader& shader, const glm::mat4& model, bool setTransInv)
{
  shader.setMat4fv("u_model", model);
  if (setTransInv) {
    shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model))));
  }
}
