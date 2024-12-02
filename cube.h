#ifndef CUBE_H
#define CUBE_H

#include "shape.h"

class Cube : public Shape
{
  public:
    Cube(const char* vShader, const char* fShader);

    void render();
    void free();

  private:
    void enterVertices();
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

#endif
