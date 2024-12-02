#include "shape.h"

#include <glad/gl.h>
#include <iostream>

#include "vertex.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shape::Shape(const char* vShader, const char* fShader)
{
  program = new Shader(vShader, fShader);
}

Shape::~Shape()
{
  delete program;
}

void Shape::setModel(glm::mat4 model)
{
  program->use();
  program->setMat4fv("model", glm::value_ptr(model));
}

void Shape::setView(glm::mat4 view)
{
  program->use();
  program->setMat4fv("view", glm::value_ptr(view));
}

void Shape::setProjection(glm::mat4 projection)
{
  program->use();
  program->setMat4fv("projection", glm::value_ptr(projection));
}

void Shape::free()
{
  glDeleteProgram(program->ID);
}
