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

  void free() override;
  void update(float dt);

  const std::vector<glm::mat4> pose() const;
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

  Animation readAnimation(const aiAnimation* scene);
  std::vector<SkeletalVertex> readBoneData(const aiMesh* mesh, BoneInfos& outBoneInfos);
  bool replaceOrDiscard(
    unsigned int boneId, float weight, SkeletalVertex &outVertexBoneData
  );

  void addBone(const aiBone* aiBone, BoneInfos& outBoneInfos);
  bool readSkeleton(const aiNode* node, const BoneInfos& boneInfos, Bone& outSkeleton);
  std::pair<int, float> getTimeFraction(const std::vector<float>& times, float dt) const;
  glm::mat4 localTransform(const BoneTransforms& transforms, const unsigned int &time) const;
  void updatePose(
    const Animation& animation,
    const Bone& skeleton,
    const float& dt,
    const glm::mat4& parentTransform,
    std::vector<glm::mat4>& output
  );
};

#endif
