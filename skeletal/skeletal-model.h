#ifndef SKELETAL_MODEL_H
#define SKELETAL_MODEL_H

#include "../model.h"
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "skeletal-mesh.h"
#include "skeletal-vertex.h"
#include "bone.h"
#include "animation.h"


struct BoneInfo
{
  unsigned int index;
  glm::mat4 offset;
};


class SkeletalModel : public Model
{
public:
  SkeletalModel();
  ~SkeletalModel();

  void free() override;
  void update(float dt);

  const std::vector<glm::mat4> pose() const;
  bool setAnimation(const unsigned int animationIndex);
  void setSpeed(const unsigned int speed);

protected:
  void processScene(const aiScene* const scene) override;

private:
  unsigned int mAnimationIndex;
  std::vector<Animation> mAnimations;
  std::vector<SkeletalMesh> mMeshes;
  std::unordered_map<std::string, BoneInfo> mBoneInfoMap;
  Bone mRootBone;
  std::vector<glm::mat4> mCurrentPose;
  unsigned int mSpeed;

  void readAnimation(const aiAnimation* scene, Animation& outAnimation);
  void readBoneData(
    const aiMesh* mesh,
    std::vector<SkeletalVertex>& outBoneData,
    std::unordered_map<std::string, BoneInfo>& outBoneInfoMap
  );
  bool replaceOrDiscard(
    const unsigned int boneId, const float weight, SkeletalVertex &outVertexBoneData
  );

  BoneInfo addBone(
    const std::string name, const glm::mat4 offset,
    std::unordered_map<std::string, BoneInfo>& outBoneInfoMap
  );
  // void normalizeWeights(std::vector<SkeletalVertex>& boneData);
  bool readSkeleton(const aiNode* node, Bone& outSkeleton);
  std::pair<int, float> getTimeFraction(const std::vector<float>& times, const float& dt) const;
  void getPose(
    Animation& animation,
    const Bone& skeleton,
    const float dt,
    const glm::mat4 &parentTransform,
    std::vector<glm::mat4>& output
  );
};

#endif
