#ifndef COLOR_H
#define COLOR_H

#include <glm/glm.hpp>

class Color : public glm::vec4 {
public:
  Color(float r, float g, float b, float a = 1.f) : glm::vec4(r, g, b, a) {}
  Color(float v) : Color(v, v, v, v) { }
  Color() : Color(0.f) { }
};

#endif
