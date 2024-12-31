#ifndef MESH_H
#define MESH_H

#include <vector>
#include "vertex.h"
#include "texture.h"
#include "shader.h"
#include "shapes.h"

class Mesh
{
public:
  Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  Mesh(const Shapes::Shape &shape) : Mesh(shape.vertices, shape.indices) {}
  virtual ~Mesh();

  void draw(const Shader &shader, const Texture* textures, const unsigned int texLen) const;
  void draw(const Shader &shader) const { draw(shader, nullptr, 0); }
  void draw(const Shader &shader, const Texture* texture) const { draw(shader, texture, 1); }
  void draw(const Shader &shader, const std::vector<Texture> &textures) const { draw(shader, &textures[0], textures.size()); }

  void free() const;

  const unsigned int vao() const;

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  const unsigned int M_INDICES_SIZE;
};

#endif
