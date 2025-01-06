#ifndef MESH_H
#define MESH_H

#include "shapes.h"
#include "vertex.h"
#include <vector>

class Mesh
{
public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  Mesh(const Shapes::Shape& shape) : Mesh(shape.vertices, shape.indices) { }
  virtual ~Mesh();

  void draw() const;
  void drawInstanced(const unsigned int instanceCount) const;

  void free() const;

  const unsigned int& vao() const { return VAO; }

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  const unsigned int M_INDICES_SIZE;
};

#endif
