#ifndef LIGHT_H
#define LIGHT_H

#include "shape.h"

class Light : public Shape
{
  public:
    Light(const char* vShader, const char* fShader);

    void render();
    void free();

  private:
    void enterVertices();
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

#endif
