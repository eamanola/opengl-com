#ifndef BONE_H
#define BONE_H

#include <string>
#include <vector>

struct Bone
{
  std::string name;
  std::vector<Bone> children = {};
};

#endif
