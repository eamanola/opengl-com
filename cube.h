#ifndef CUBE_H
#define CUBE_H

#include "shape.h"

class Cube : public Shape
{
  public:
    Cube(
      const char* vShader = "./shaders/lighting.glvs",
      const char* fShader = "./shaders/lighting.glfs"
    );

    void render() const;
    void free();

  private:
    void enterVertices();
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int diffuseMap;
    unsigned int specularMap;
};

#endif
