#ifndef LIGHT_H
#define LIGHT_H

#include "shape.h"

class Light : public Shape
{
  public:
    Light(
      const char* vShader = "./shaders/light.glvs",
      const char* fShader = "./shaders/light.glfs"
    );

    void render();
    void free();

  private:
    void enterVertices();
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

#endif
