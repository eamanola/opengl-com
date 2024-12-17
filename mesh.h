#ifndef MESH_H
#define MESH_H

#include <vector>
#include "vertex.h"
#include "texture.h"
#include "shader.h"

class Mesh
{
public:
  Mesh(
    std::vector<Vertex> vertices,
    std::vector<unsigned int> indices
  );
  virtual ~Mesh();

  void draw(Shader &shader, std::vector<Texture> &textures);
  void free();

  const unsigned int vao() const;

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

#endif
