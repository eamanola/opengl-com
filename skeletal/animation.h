#ifndef ANIMATION_H
#define ANIMATION_H

#include <unordered_map>
#include <string>
#include "bone-transform.h"

struct Animation
{
  float duration;
  float ticksPerSecond = 1.f;
  std::unordered_map<std::string, BoneTransform> boneTransforms = {};
};


#endif
