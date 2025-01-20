#include "shapes.h"

void Shapes_calculateTangent(Vertex& v1, Vertex& v2, Vertex& v3)
{
  glm::vec3 edge1 = v2.position - v1.position;
  glm::vec3 edge2 = v3.position - v1.position;

  glm::vec2 dUV1 = v2.texCoords - v1.texCoords;
  glm::vec2 dUV2 = v3.texCoords - v1.texCoords;

  float frac = 1.f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

  Tangent tangent {
    dUV2.y * edge1.x - dUV1.y * edge2.x,
    dUV2.y * edge1.y - dUV1.y * edge2.y,
    dUV2.y * edge1.z - dUV1.y * edge2.z,
  };
  tangent *= frac;

  // std::cout << v1.position.x << ", " << v1.position.y << ", " << v1.position.z << " " <<
  // tangent.x
  //           << ", " << tangent.y << ", " << tangent.z << "\n"
  //           << v2.position.x << ", " << v2.position.y << ", " << v2.position.z << "\n"
  //           << v3.position.x << ", " << v3.position.y << ", " << v3.position.z << "\n"
  //           << "\n";

  // bitan
  // x = -dUV2.x * edge1.x + dUV1.x * edge2.x
  // y = -dUV2.x * edge1.y + dUV1.x * edge2.y
  // z = -dUV2.x * edge1.z + dUV1.x * edge2.z
  // calc in shader: bitan = cross(normal, tangent);
  // note: for non-ortho tangents,
  // convert: tangent = normalize(tangent - dot(tangent, normal) * normal); //Gram-Schmidt process,

  v1.tangent = v2.tangent = v3.tangent = tangent;
}

void Shapes_calculateTangents(Shapes::Shape& shape)
{
  std::vector<Vertex>& vertices = shape.vertices;
  const std::vector<unsigned int>& indices = shape.indices;

  for (unsigned int i = 0; i < indices.size(); i += 3) {
    Vertex& v1 = vertices[indices[i + 0]];
    Vertex& v2 = vertices[indices[i + 1]];
    Vertex& v3 = vertices[indices[i + 2]];

    Shapes_calculateTangent(v1, v2, v3);
  }
}

Shapes::Shape Shapes::withTangents(const Shapes::Shape& shape)
{
  Shape out { shape };

  Shapes_calculateTangents(out);

  return out;
}
