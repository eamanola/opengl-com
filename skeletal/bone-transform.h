#ifndef BONETRANSFORM_H
#define BONETRANSFORM_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct BoneTransform
{
  std::vector<float> positionTimestamps = {};
  std::vector<float> rotationTimestamps = {};
  std::vector<float> scaleTimestamps = {};

  std::vector<glm::vec3> positions = {};
  std::vector<glm::quat> rotations = {};
  std::vector<glm::vec3> scales = {};
};

#endif
