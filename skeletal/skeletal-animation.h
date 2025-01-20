#ifndef SKELETA_ANIMATION_H
#define SKELETA_ANIMATION_H

#include "array.h"
#include "skeletal-model.h"
#include <glm/glm.hpp>

class SkeletalAnimation
{
public:
  SkeletalAnimation(const SkeletalModel& model) : mModel(model)
  {
    mAnimationIndex = 0;
    mCurrentPose.resize(mModel.boneCount(), glm::mat4(1.0));
  }
  SkeletalAnimation(const char* path) : SkeletalAnimation(SkeletalModel(path)) { }
  ~SkeletalAnimation() { }

  void free() const { mModel.free(); };
  void draw(const Shader& shader) const { mModel.draw(shader); };

  const Animation* setAnimation(unsigned int animationIndex);
  void updatePose(const float& time);
  const Array<glm::mat4> pose() const { return Array(mCurrentPose); }

private:
  SkeletalModel mModel;
  void updatePose(
    const Animation& animation,
    const Bone& skeleton,
    const float& dt,
    const glm::mat4& parentTransform,
    std::vector<glm::mat4>& output
  ) const;
  std::pair<int, float> getTimeFraction(const std::vector<float>& times, float dt) const;
  glm::mat4 localTransform(const BoneTransforms& transforms, const float& time) const;

  std::vector<glm::mat4> mCurrentPose;
  unsigned int mAnimationIndex;
};

#endif
