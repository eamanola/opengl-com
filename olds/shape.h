#ifndef Shape_H
#define Shape_H

#include "shader.h"
#include <glm/glm.hpp>

class Shape
{
private:
  Shader* program;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int wallTexture;
  unsigned int faceTexture;
  void enterVertices();
  unsigned int createTexture(const char* path, GLenum format = GL_RGB);
public:
  Shape(/* args */);
  ~Shape();

  void render();
  void free();
  void setView(glm::mat4 view);
  void setProjection(glm::mat4 projection);
};

#endif
