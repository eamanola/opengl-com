#ifndef COLOR_H
#define COLOR_H

#include <glm/glm.hpp>

typedef glm::vec4 Color;

struct PhongColor {
  Color ambient;
  Color diffuse;
  Color specular;
};

#endif
