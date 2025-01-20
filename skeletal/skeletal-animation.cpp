#include "skeletal-animation.h"

void SkeletalAnimation::updatePose(const float& time)
{
  glm::mat4 identity = glm::mat4(1.0f);
  const Array<Animation> animations = mModel.animations();
  const Bone& rootBone = mModel.rootBone();
  updatePose(animations[mAnimationIndex], rootBone, time, identity, mCurrentPose);
}

void SkeletalAnimation::updatePose(
  const Animation& animation,
  const Bone& bone,
  const float& timeInSec,
  const glm::mat4& parentTransform,
  std::vector<glm::mat4>& output
) const
{
  // const BoneInfo& boneInfo = mBoneInfoMap[bone.name];

#ifdef WITH_TRANSFORM
  glm::mat4 local = bone.transform;
#else
  glm::mat4 local = glm::mat4(1.f);
#endif
  if (animation.boneTransforms.find(bone.name) != animation.boneTransforms.end()) {
    const BoneTransforms& animTransforms = animation.boneTransforms.at(bone.name);
    const float animTime = fmod(timeInSec * animation.ticksPerSecond, animation.duration);

    local = localTransform(animTransforms, animTime);
  }

  glm::mat4 globalTransform = parentTransform * local;

  // glm::mat4 globalInverseTransform = glm::inverse(mRootBone.transform);

  output[bone.index] = /* globalInverseTransform * */ globalTransform * bone.offset;

  for (unsigned int i = 0; i < bone.children.size(); i++) {
    updatePose(animation, bone.children[i], timeInSec, globalTransform, output);
  }
}

std::pair<int, float>
SkeletalAnimation::getTimeFraction(const std::vector<float>& times, float animTime) const
{
  unsigned int segment = 0;
  while (animTime >= times[segment])
    segment++;

  if (segment == 0) {
    return { 1, 0 };
  }

  float start = times[segment - 1];
  float end = times[segment];
  float frac = (animTime - start) / (end - start);

  return { segment, frac };
}

glm::mat4
SkeletalAnimation::localTransform(const BoneTransforms& transforms, const float& time) const
{
  const glm::mat4 IDENTITY = glm::mat4(1.f);
  glm::mat4 positionMat = IDENTITY;
  glm::mat4 rotationMat = IDENTITY;
  glm::mat4 scaleMat = IDENTITY;

  const unsigned int posSize = transforms.positions.size();
  if (posSize == 1) {
    // check timestamp?
    positionMat = glm::translate(IDENTITY, transforms.positions[0]);
  } else if (posSize > 1) {
    const std::pair<unsigned int, float> fp = getTimeFraction(transforms.positionTimestamps, time);
    glm::vec3 position1 = transforms.positions[fp.first - 1];
    glm::vec3 position2 = transforms.positions[fp.first];
    glm::vec3 position = glm::mix(position1, position2, fp.second);

    positionMat = glm::translate(IDENTITY, position);
  }

  const unsigned int rotSize = transforms.rotations.size();
  if (rotSize == 1) {
    rotationMat = glm::toMat4(glm::normalize(transforms.rotations[0]));
  } else if (rotSize > 1) {
    const std::pair<unsigned int, float> fp = getTimeFraction(transforms.rotationTimestamps, time);
    glm::quat rotation1 = transforms.rotations[fp.first - 1];
    glm::quat rotation2 = transforms.rotations[fp.first];
    glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);
    rotationMat = glm::toMat4(glm::normalize(rotation));
  }

  const unsigned int scaSize = transforms.scales.size();
  if (scaSize == 1) {
    scaleMat = glm::scale(IDENTITY, transforms.scales[0]);
  } else if (scaSize > 1) {
    const std::pair<unsigned int, float> fp = getTimeFraction(transforms.scaleTimestamps, time);
    glm::vec3 scale1 = transforms.scales[fp.first - 1];
    glm::vec3 scale2 = transforms.scales[fp.first];
    glm::vec3 scale = glm::mix(scale1, scale2, fp.second);
    scaleMat = glm::scale(IDENTITY, scale);
  }

  return positionMat * rotationMat * scaleMat;
}

const Animation* SkeletalAnimation::setAnimation(const unsigned int animationIndex)
{
  const Array<Animation> animations = mModel.animations();
  if (animationIndex < animations.size()) {
    mAnimationIndex = animationIndex;
    return &animations[animationIndex];
  }

  return nullptr;
}
