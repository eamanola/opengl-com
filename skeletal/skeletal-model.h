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
  SkeletalModel(const char* path) : Model()
  {
    // call from this to get processScene
    loadModel(path);
  };
  ~SkeletalModel();

  void free() const override;

  unsigned int addAnimation(Animation animation);

  const std::vector<Animation>& animations() { return mAnimations; }
  const Bone& rootBone() { return mRootBone; }
  const unsigned int& boneCount() { return mBoneCount; }

private:
  unsigned int mBoneCount;
  std::vector<Animation> mAnimations;
  Bone mRootBone;

  void processScene(const aiScene* scene) override;
  std::vector<SkeletalMesh> mMeshes;
  Animation readAnimation(const aiAnimation* scene) const;
  std::vector<SkeletalVertex> readBoneData(const aiMesh* mesh, BoneInfos& outBoneInfos) const;
  unsigned int replaceIndex(const float* weights, const float& weight) const;
  void addBone(const aiBone* aiBone, BoneInfos& outBoneInfos) const;
  bool readSkeleton(const aiNode* node, const BoneInfos& boneInfos, Bone& outSkeleton) const;
};

#endif
