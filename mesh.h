#ifndef MESH_H
#define MESH_H

#include "shapes.h"
#include "vertex.h"
#include <glad/gl.h>
#include <vector>

class Mesh
{
public:
  enum AttribType { FLOAT = GL_FLOAT, UNSIGNED_INT = GL_UNSIGNED_INT };
  enum BufferUsage { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW };
  struct VertexAttribPointer {
    unsigned int location;
    std::size_t size;
    AttribType type = AttribType::FLOAT;
    GLboolean normalized = GL_FALSE;
    std::size_t stride;
    void* offset;
    unsigned int divisor = 0;
  };

  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  Mesh(const Shapes::Shape& shape) : Mesh(shape.vertices, shape.indices) { }
  virtual ~Mesh();

  void draw() const;
  void drawInstanced(const unsigned int instanceCount) const;

  void free() const;

  const unsigned int& vao() const { return VAO; }

  bool addBuffer(
    unsigned int& bufferId,
    const void* data,
    const std::size_t size,
    const std::vector<VertexAttribPointer>& attibPointers,
    BufferUsage usage = BufferUsage::STATIC
  );

private:
  unsigned int VAO;
  void setupBuffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  const unsigned int M_INDICES_SIZE;
  std::vector<unsigned int> mBuffers;
};

#endif
