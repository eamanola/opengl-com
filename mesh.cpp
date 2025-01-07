#include "mesh.h"

#include "gl-utils/gl-utils.h"
#include "shaders/attrib-locations.h"
#include <glad/gl.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) :
  M_INDICES_SIZE(indices.size())
{
  setupBuffers(vertices, indices);
}

Mesh::~Mesh() { }

void Mesh::setupBuffers(
  const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices
)
{
  glGenVertexArrays(1, &VAO);

  std::vector<VertexAttribPointer> attribPointers;

  VertexAttribPointer positions = {
    .location = ATTRIB_LOCATIONS::POSITION,
    .size = 3,
    .stride = sizeof(Vertex),
    .offset = (void*)offsetof(Vertex, position),
  };
  attribPointers.push_back(positions);

  // glGenBuffers(1, &VBO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  // glVertexAttribPointer(
  //   ATTRIB_LOCATIONS::POSITION,
  //   3,
  //   GL_FLOAT,
  //   GL_FALSE,
  //   sizeof(Vertex),
  //   (void*)offsetof(Vertex, position)
  // );
  // glEnableVertexAttribArray(ATTRIB_LOCATIONS::POSITION);

  bool enableNormals = false;
  for (Vertex vertex : vertices) {
    if (vertex.normal != glm::vec3(0)) {
      enableNormals = true;
      break;
    }
  }

  if (enableNormals) {
    VertexAttribPointer normals = {
      .location = ATTRIB_LOCATIONS::NORMAL,
      .size = 3,
      .stride = sizeof(Vertex),
      .offset = (void*)offsetof(Vertex, normal),
    };
    attribPointers.push_back(normals);
  }

  bool enableTexCoords = false;
  for (Vertex vertex : vertices) {
    if (vertex.texCoords != glm::vec2(0)) {
      enableTexCoords = true;
      break;
    }
  }

  if (enableTexCoords) {
    VertexAttribPointer texCoords = {
      .location = ATTRIB_LOCATIONS::TEX_COORDS,
      .size = 2,
      .stride = sizeof(Vertex),
      .offset = (void*)offsetof(Vertex, texCoords),
    };
    attribPointers.push_back(texCoords);
  }

  unsigned int VBO;
  addBuffer(VBO, &vertices[0], sizeof(Vertex) * vertices.size(), attribPointers);
  glBindVertexArray(VAO);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  mBuffers.push_back(EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW
  );
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // keep bound
  glBindVertexArray(0);
}

bool Mesh::addBuffer(
  unsigned int& bufferId,
  const void* data,
  const std::size_t size,
  const std::vector<VertexAttribPointer>& attibPointers,
  const BufferUsage usage
)
{
  glBindVertexArray(VAO);
  glGenBuffers(1, &bufferId);
  glBindBuffer(GL_ARRAY_BUFFER, bufferId);

  glBufferData(GL_ARRAY_BUFFER, size, data, usage);

  for (VertexAttribPointer v : attibPointers) {
    glVertexAttribPointer(v.location, v.size, v.type, v.normalized, v.stride, v.offset);

    if (v.divisor > 0) {
      glVertexAttribDivisor(v.location, v.divisor);
    }

    glEnableVertexAttribArray(v.location);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0); // Saved to VAO with glVertexAttribPointer
  glBindVertexArray(0);

  if (GLUtils::noErrors()) {
    mBuffers.push_back(bufferId);
    return true;
  } else {
    glDeleteBuffers(1, &bufferId);
    bufferId = 0;
    return false;
  }
}

void Mesh::draw() const
{
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, M_INDICES_SIZE, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::drawInstanced(const unsigned int instanceCount) const
{
  glBindVertexArray(VAO);
  glDrawElementsInstanced(GL_TRIANGLES, M_INDICES_SIZE, GL_UNSIGNED_INT, 0, instanceCount);
  glBindVertexArray(0);
}

void Mesh::free() const
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(mBuffers.size(), &mBuffers[0]);
}
