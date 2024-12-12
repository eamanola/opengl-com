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
    std::vector<unsigned int> indices,
    std::vector<Texture> textures
  );
  virtual ~Mesh();

  void draw(Shader &shader);
  void free();

  unsigned int VAO;

private:
  unsigned int VBO, EBO;
  void setupMesh();

  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture>      textures;
};

#endif
