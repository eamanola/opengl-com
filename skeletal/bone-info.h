#ifndef BONE_INFO_H
#define BONE_INFO_H

#include <glm/glm.hpp>

struct BoneInfo
{
  unsigned int index;
  glm::mat4 offset;
};

struct BoneInfos : public std::unordered_map<std::string, BoneInfo>
{

};

#endif
