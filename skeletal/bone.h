#ifndef BONE_H
#define BONE_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

// #define WITH_TRANSFORM

struct Bone
{
  std::string name;
  #ifdef WITH_TRANSFORM
  glm::mat4 transform;
  #endif
  std::vector<Bone> children = {};
};

#endif
