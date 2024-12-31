#ifndef SKELETA_ANIMATION_H
#define SKELETA_ANIMATION_H

#include "skeletal-model.h"
#include <glm/glm.hpp>

class SkeletalAnimation : public SkeletalModel
{
public:
  SkeletalAnimation(const char* path) : SkeletalModel(path)
  {
    mAnimationIndex = 0;
    mCurrentPose.resize(boneCount(), glm::mat4(1.0));
  }
  ~SkeletalAnimation() {}

  const Animation* setAnimation(unsigned int animationIndex);
  void updatePose(const float& time);
  const std::vector<glm::mat4>& pose() const { return mCurrentPose; }

private:
  void updatePose(
    const Animation& animation,
    const Bone& skeleton,
    const float& dt,
    const glm::mat4& parentTransform,
    std::vector<glm::mat4>& output
  ) const;
  std::pair<int, float> getTimeFraction(const std::vector<float>& times, float dt) const;
  glm::mat4 localTransform(const BoneTransforms& transforms, const unsigned int &time) const;

  std::vector<glm::mat4> mCurrentPose;
  unsigned int mAnimationIndex;
};


#endif
