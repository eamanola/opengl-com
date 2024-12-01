#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shader.h"

class Triangle
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
  Triangle(/* args */);
  ~Triangle();

  void render();
  void free();
};

#endif
