#ifndef SKELETAL_VERTEX_H
#define SKELETAL_VERTEX_H

#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

struct SkeletalVertex
{
  unsigned int boneIds[MAX_BONE_INFLUENCE] = {};
  float boneWeights[MAX_BONE_INFLUENCE] = {};
};

#endif
