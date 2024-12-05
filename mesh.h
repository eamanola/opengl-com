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
  ~Mesh();

  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture>      textures;

  void draw(Shader &shader);
  void free();

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};

#endif
