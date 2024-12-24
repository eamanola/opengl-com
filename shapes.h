#ifndef SHAPES
#define SHAPES

#include <vector>
#include "vertex.h"

namespace Shapes
{
  struct Shape {
    const std::vector<Vertex> vertices;
    const std::vector<unsigned int> indices;
  };

  const Shape QUAD =
  {
    .vertices = {
      { .position = { -0.5f, -0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 0.f, 0.f } },
      { .position = {  0.5f, -0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 1.f, 0.f } },
      { .position = {  0.5f,  0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 1.f, 1.f } },
      { .position = { -0.5f,  0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 0.f, 1.f } }
    },
    .indices = {
      0, 1, 2,
      0, 2, 3
    }
  };

  const Shape CUBE = {
    std::vector<Vertex> {
      { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.f,  0.f, -1.0f }, .texCoords { 0.f, 0.f } },
      { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.f,  0.f, -1.0f }, .texCoords { 1.f, 0.f } },
      { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.f,  0.f, -1.0f }, .texCoords { 1.f, 1.f } },
      { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.f,  0.f, -1.0f }, .texCoords { 0.f, 1.f } },

      { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.f,  0.f,  1.0f }, .texCoords { 0.f, 0.f } },
      { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.f,  0.f,  1.0f }, .texCoords { 1.f, 0.f } },
      { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.f,  0.f,  1.0f }, .texCoords { 1.f, 1.f } },
      { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.f,  0.f,  1.0f }, .texCoords { 0.f, 1.f } },

      { .position { -0.5f, -0.5f, -0.5f }, .normal { -1.0f,  0.f,  0.f }, .texCoords { 0.f, 0.f } },
      { .position { -0.5f,  0.5f, -0.5f }, .normal { -1.0f,  0.f,  0.f }, .texCoords { 1.f, 0.f } },
      { .position { -0.5f,  0.5f,  0.5f }, .normal { -1.0f,  0.f,  0.f }, .texCoords { 1.f, 1.f } },
      { .position { -0.5f, -0.5f,  0.5f }, .normal { -1.0f,  0.f,  0.f }, .texCoords { 0.f, 1.f } },

      { .position {  0.5f, -0.5f, -0.5f }, .normal {  1.0f,  0.f,  0.f }, .texCoords { 0.f, 0.f } },
      { .position {  0.5f,  0.5f, -0.5f }, .normal {  1.0f,  0.f,  0.f }, .texCoords { 1.f, 0.f } },
      { .position {  0.5f,  0.5f,  0.5f }, .normal {  1.0f,  0.f,  0.f }, .texCoords { 1.f, 1.f } },
      { .position {  0.5f, -0.5f,  0.5f }, .normal {  1.0f,  0.f,  0.f }, .texCoords { 0.f, 1.f } },

      { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.f, -1.0f,  0.f }, .texCoords { 0.f, 0.f } },
      { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.f, -1.0f,  0.f }, .texCoords { 1.f, 0.f } },
      { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.f, -1.0f,  0.f }, .texCoords { 1.f, 1.f } },
      { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.f, -1.0f,  0.f }, .texCoords { 0.f, 1.f } },

      { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.f,  1.0f,  0.f }, .texCoords { 0.f, 0.f } },
      { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.f,  1.0f,  0.f }, .texCoords { 1.f, 0.f } },
      { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.f,  1.0f,  0.f }, .texCoords { 1.f, 1.f } },
      { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.f,  1.0f,  0.f }, .texCoords { 0.f, 1.f } }
    },
    std::vector<unsigned int> {
      0,  2,  1,  0,  3,  2,
      4,  5,  6,  4,  6,  7,
      8, 10,  9,  8, 11, 10,
      12, 13, 14, 12, 14, 15,
      16, 17, 18, 16, 18, 19,
      20, 22, 21, 20, 23, 22
    }
  };

  const Shape CUBE_ONLY_P = {
    std::vector<Vertex> ({
      { .position = {  0.5f,  0.5f,  0.5f } },
      { .position = {  0.5f, -0.5f,  0.5f } },
      { .position = { -0.5f, -0.5f,  0.5f } },
      { .position = { -0.5f,  0.5f,  0.5f } },
      { .position = {  0.5f,  0.5f, -0.5f } },
      { .position = {  0.5f, -0.5f, -0.5f } },
      { .position = { -0.5f, -0.5f, -0.5f } },
      { .position = { -0.5f,  0.5f, -0.5f } }
    }),
    std::vector<unsigned int>({
      0, 1, 2, 0, 2, 3, // front
      4, 5, 1, 4, 1, 0, // right
      7, 6, 5, 7, 5, 4, // back
      3, 2, 6, 3, 6, 7, // left
      4, 0, 3, 4, 3, 7, // top
      1, 5, 6, 1, 6, 2  // bottom
    })
  };
}

#endif
