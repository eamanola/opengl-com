#ifndef SKELETAL_MODEL_H
#define SKELETAL_MODEL_H

#include "../model.h"
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "skeletal-mesh.h"
#include "skeletal-vertex.h"
#include "bone.h"
#include "animation.h"
#include "bone-info.h"

class SkeletalModel : public Model
{
public:
  SkeletalModel();
  ~SkeletalModel();

  virtual void free() override;
  virtual void draw(const Shader &shader) override;
  virtual void update(const float &time) override;

  const std::vector<glm::mat4>& pose() const;
  const Animation* setAnimation(unsigned int animationIndex);
  unsigned int addAnimation(Animation animation);

protected:
  void processScene(const aiScene* scene) override;

private:
  unsigned int mAnimationIndex;
  std::vector<Animation> mAnimations;
  std::vector<SkeletalMesh> mMeshes;
  Bone mRootBone;
  std::vector<glm::mat4> mCurrentPose;

  Animation readAnimation(const aiAnimation* scene) const;
  std::vector<SkeletalVertex> readBoneData(const aiMesh* mesh, BoneInfos& outBoneInfos) const;
  unsigned int replaceIndex(const float* weights, const float& weight) const;
  void addBone(const aiBone* aiBone, BoneInfos& outBoneInfos) const;
  bool readSkeleton(const aiNode* node, const BoneInfos& boneInfos, Bone& outSkeleton) const;
  std::pair<int, float> getTimeFraction(const std::vector<float>& times, float dt) const;
  glm::mat4 localTransform(const BoneTransforms& transforms, const unsigned int &time) const;
  void updatePose(
    const Animation& animation,
    const Bone& skeleton,
    const float& dt,
    const glm::mat4& parentTransform,
    std::vector<glm::mat4>& output
  ) const;
};

#endif
