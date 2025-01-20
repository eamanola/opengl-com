#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

typedef glm::vec3 Position;
typedef glm::vec3 Normal;
typedef glm::vec2 TexCoords;
typedef glm::vec3 Tangent;

struct Vertex {
  Position position;
  Normal normal;
  TexCoords texCoords;
  Tangent tangent;
};

#endif
