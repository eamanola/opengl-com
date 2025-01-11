#ifndef U_MODEL_H
#define U_MODEL_H

#include "mesh.h"
#include "shader.h"
#include <glm/glm.hpp>

namespace u_model
{

unsigned int setInstancedModels(const unsigned int vao, const std::vector<glm::mat4>& models);
void setUModel(const Shader& shader, const glm::mat4& model, bool setTransInv = true);

} // namespace u_model

#endif
