#ifndef LIGHT_H
#define LIGHT_H

#include "shape.h"

class Light : public Shape
{
  public:
    Light(
      const char* vShader = "./shaders/plain.glvs",
      const char* fShader = "./shaders/single-color.glfs"
    );

    void render() const;
    void free();

  private:
    void enterVertices();
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

#endif
