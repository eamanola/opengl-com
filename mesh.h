#ifndef MESH_H
#define MESH_H

#include <vector>
#include "vertex.h"
#include "texture.h"
#include "shader.h"

class Mesh
{
public:
  Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  virtual ~Mesh();

  void draw(Shader &shader, const Texture* textures, const unsigned int texLen);
  void draw(Shader &shader);
  void draw(Shader &shader, const Texture* texture);
  void draw(Shader &shader, const std::vector<Texture> &textures);
  void free();

  const unsigned int vao() const;

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  const unsigned int M_INDICES_SIZE;
};

#endif
