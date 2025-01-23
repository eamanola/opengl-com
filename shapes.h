#ifndef SHAPES_H
#define SHAPES_H

#include "vertex.h"
#include <vector>

namespace Shapes
{

struct Shape {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

Shape withTangents(const Shape& shape);

const Shape QUAD {
  {
   // clang-format off
    { .position { -1.f, -1.f, 0.f }, .normal  { 0.f, 0.f, 1.f }, .texCoords { 0.f, 0.f } },
    { .position {  1.f, -1.f, 0.f }, .normal  { 0.f, 0.f, 1.f }, .texCoords { 1.f, 0.f } },
    { .position {  1.f,  1.f, 0.f }, .normal  { 0.f, 0.f, 1.f }, .texCoords { 1.f, 1.f } },
    { .position { -1.f,  1.f, 0.f }, .normal  { 0.f, 0.f, 1.f }, .texCoords { 0.f, 1.f } },
   // clang-format on
  },
  {
   // clang-format off
    0, 1, 2,
    0, 2, 3,
   // clang-format on
  },
};

const Shape CUBE {
  {
   // clang-format off
    { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.f,  0.f, -1.f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.f,  0.f, -1.f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.f,  0.f, -1.f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.f,  0.f, -1.f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.f,  0.f,  1.f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.f,  0.f,  1.f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.f,  0.f,  1.f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.f,  0.f,  1.f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f, -0.5f, -0.5f }, .normal { -1.f,  0.f,  0.f }, .texCoords { 0.f, 0.f } },
    { .position { -0.5f,  0.5f, -0.5f }, .normal { -1.f,  0.f,  0.f }, .texCoords { 1.f, 0.f } },
    { .position { -0.5f,  0.5f,  0.5f }, .normal { -1.f,  0.f,  0.f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f, -0.5f,  0.5f }, .normal { -1.f,  0.f,  0.f }, .texCoords { 0.f, 1.f } },

    { .position {  0.5f, -0.5f, -0.5f }, .normal {  1.f,  0.f,  0.f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  1.f,  0.f,  0.f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  1.f,  0.f,  0.f }, .texCoords { 1.f, 1.f } },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  1.f,  0.f,  0.f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f, -0.5f, -0.5f }, .normal {  0.f, -1.f,  0.f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f, -0.5f, -0.5f }, .normal {  0.f, -1.f,  0.f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f, -0.5f,  0.5f }, .normal {  0.f, -1.f,  0.f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f, -0.5f,  0.5f }, .normal {  0.f, -1.f,  0.f }, .texCoords { 0.f, 1.f } },

    { .position { -0.5f,  0.5f, -0.5f }, .normal {  0.f,  1.f,  0.f }, .texCoords { 0.f, 0.f } },
    { .position {  0.5f,  0.5f, -0.5f }, .normal {  0.f,  1.f,  0.f }, .texCoords { 1.f, 0.f } },
    { .position {  0.5f,  0.5f,  0.5f }, .normal {  0.f,  1.f,  0.f }, .texCoords { 1.f, 1.f } },
    { .position { -0.5f,  0.5f,  0.5f }, .normal {  0.f,  1.f,  0.f }, .texCoords { 0.f, 1.f } },
   // clang-format on

  },
  {
   // clang-format off
     0,  2,  1,  0,  3,  2,
     4,  5,  6,  4,  6,  7,
     8, 10,  9,  8, 11, 10,
    12, 13, 14, 12, 14, 15,
    16, 17, 18, 16, 18, 19,
    20, 22, 21, 20, 23, 22,
   // clang-format on
  }
};

const Shape CUBE_ONLY_P {
  {
   // clang-format off
    { .position {  0.5f,  0.5f,  0.5f } },
    { .position {  0.5f, -0.5f,  0.5f } },
    { .position { -0.5f, -0.5f,  0.5f } },
    { .position { -0.5f,  0.5f,  0.5f } },
    { .position {  0.5f,  0.5f, -0.5f } },
    { .position {  0.5f, -0.5f, -0.5f } },
    { .position { -0.5f, -0.5f, -0.5f } },
    { .position { -0.5f,  0.5f, -0.5f } },
   // clang-format on
  },
  {
   // clang-format off
    0, 1, 2, 0, 2, 3, // front
    4, 5, 1, 4, 1, 0, // right
    7, 6, 5, 7, 5, 4, // back
    3, 2, 6, 3, 6, 7, // left
    4, 0, 3, 4, 3, 7, // top
    1, 5, 6, 1, 6, 2, // bottom
   // clang-format on
  },
};

const Shape SKYBOX {
  {
   // clang-format off
    { .position {  1.f,  1.f,  1.f } }, // 0 +++
    { .position {  1.f, -1.f,  1.f } }, // 1 +-+
    { .position { -1.f, -1.f,  1.f } }, // 2 --+
    { .position { -1.f,  1.f,  1.f } }, // 3 -++
    { .position {  1.f,  1.f, -1.f } }, // 4 ++-
    { .position {  1.f, -1.f, -1.f } }, // 5 +--
    { .position { -1.f, -1.f, -1.f } }, // 6 ---
    { .position { -1.f,  1.f, -1.f } }, // 7 -+-
   // clang-format on
  },
  {
   // clang-format off
    7, 6, 5, 5, 4, 7, // front
    2, 6, 7, 7, 3, 2, // left
    5, 1, 0, 0, 4, 5, // right
    2, 3, 0, 0, 1, 2, // back
    7, 4, 0, 0, 3, 7, // top
    6, 2, 5, 5, 2, 1, // bottom
   // clang-format on
  },
};

} // namespace Shapes

#endif
