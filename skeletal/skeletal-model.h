#ifndef SKELETAL_MODEL_H
#define SKELETAL_MODEL_H

#include "animation.h"
#include "array.h"
#include "bone-info.h"
#include "bone.h"
#include "model.h"
#include "skeletal-mesh.h"
#include "skeletal-vertex.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>

class SkeletalModel : public Model
{
public:
  SkeletalModel(const char* path) : Model()
  {
    // call from this to get processScene
    loadModel(path);
  };
  ~SkeletalModel() { }

  unsigned int addAnimation(Animation animation);

  const Array<Animation> animations() const { return Array(mAnimations); }
  const Bone& rootBone() const { return mRootBone; }
  const unsigned int& boneCount() const { return mBoneCount; }

private:
  unsigned int mBoneCount;
  std::vector<Animation> mAnimations;
  Bone mRootBone;

  void processScene(const aiScene* scene) override;
  Animation readAnimation(const aiAnimation* scene) const;
  std::vector<SkeletalVertex> readBoneData(const aiMesh* mesh, BoneInfos& outBoneInfos) const;
  unsigned int replaceIndex(const float* weights, const float& weight) const;
  void addBone(const aiBone* aiBone, BoneInfos& outBoneInfos) const;
  bool readSkeleton(const aiNode* node, const BoneInfos& boneInfos, Bone& outSkeleton) const;
};

#endif
