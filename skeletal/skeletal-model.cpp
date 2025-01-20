#include "skeletal-model.h"
#include "assimp-2-glm.h"
#include <glm/gtx/quaternion.hpp>

void SkeletalModel::processScene(const aiScene* scene)
{
  Model::processScene(scene);

  if (scene->mNumAnimations == 0) {
    std::cout << "err: no animations available" << std::endl;
    return;
  }

  for (unsigned int i = 0; i < scene->mNumAnimations; i++) {

    aiAnimation* anim = scene->mAnimations[i];
    // std::cout << "info:" << i << " " << anim->mName.C_Str() << std::endl;

    addAnimation(readAnimation(anim));
  }

  BoneInfos boneInfos;
  const Mesh* meshes = this->meshes();
  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    aiMesh* aiMesh = scene->mMeshes[i];
    // std::cout << aiMesh->mName.C_Str() << std::endl;

    std::vector<SkeletalVertex> boneData = readBoneData(aiMesh, boneInfos);
    SkeletalMesh sMesh(meshes[i].vao(), boneData);
  }

  readSkeleton(scene->mRootNode, boneInfos, mRootBone);

  mBoneCount = boneInfos.size();

  // for (const std::pair<const std::string, BoneInfo>& n : mBoneInfoMap)
  //   std::cout << n.first << "\n"; // , n.second);
}

Animation SkeletalModel::readAnimation(const aiAnimation* anim) const
{
  Animation animation {
    .duration = (float)anim->mDuration,
    .ticksPerSecond = anim->mTicksPerSecond != 0.f ? (float)anim->mTicksPerSecond : 1.f,
    .boneTransforms = {},
  };

  for (unsigned int i = 0; i < anim->mNumChannels; i++) {
    const aiNodeAnim* channel = anim->mChannels[i];
    const std::string boneName = channel->mNodeName.C_Str();
    BoneTransforms transforms;

    for (unsigned int j = 0; j < channel->mNumPositionKeys; j++) {
      transforms.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
      transforms.positions.push_back(Assimp2glm::vec3(channel->mPositionKeys[j].mValue));
    }

    for (unsigned j = 0; j < channel->mNumRotationKeys; j++) {
      transforms.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
      transforms.rotations.push_back(Assimp2glm::quat(channel->mRotationKeys[j].mValue));
    }

    for (unsigned j = 0; j < channel->mNumScalingKeys; j++) {
      transforms.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
      transforms.scales.push_back(Assimp2glm::vec3(channel->mScalingKeys[j].mValue));
    }

    assert(transforms.positions.size() || transforms.rotations.size() || transforms.scales.size());
    animation.boneTransforms[boneName] = transforms;
  }

  return animation;
}

std::vector<SkeletalVertex>
SkeletalModel::readBoneData(const aiMesh* mesh, BoneInfos& outBoneInfos) const
{
  std::vector<SkeletalVertex> boneData(mesh->mNumVertices, SkeletalVertex {});

  for (unsigned int i = 0; i < mesh->mNumBones; i++) {
    const aiBone* bone = mesh->mBones[i];

    addBone(bone, outBoneInfos);

    const BoneInfo& boneInfo = outBoneInfos[bone->mName.C_Str()];

    for (unsigned int j = 0; j < bone->mNumWeights; j++) {
      unsigned int vertexId = bone->mWeights[j].mVertexId;
      float weight = bone->mWeights[j].mWeight;

      // has no effect
      if (weight == 0.f) {
        continue;
      }

      unsigned int bwi = 0;
      for (; bwi < MAX_BONE_INFLUENCE; bwi++) {
        if (boneData[vertexId].boneWeights[bwi] == 0.f) {
          boneData[vertexId].boneIds[bwi] = boneInfo.index;
          boneData[vertexId].boneWeights[bwi] = weight;
          break;
        }
      }

      if (bwi == MAX_BONE_INFLUENCE) {
        const int rIndex = replaceIndex(boneData[vertexId].boneWeights, weight);
        if (rIndex < MAX_BONE_INFLUENCE) {
          boneData[vertexId].boneIds[rIndex] = boneInfo.index;
          boneData[vertexId].boneWeights[rIndex] = weight;
        }
      }
    }
  }

  // should come normalized
  // normalizeWeights(boneData);

  return boneData;
}

unsigned int SkeletalModel::replaceIndex(const float* weights, const float& weight) const
{
  for (unsigned int i = 0; i < MAX_BONE_INFLUENCE; i++) {
    if (weights[i] < weight) {
      return i;
    }
  }

  return MAX_BONE_INFLUENCE;
}
// void SkeletalModel::normalizeWeights(std::vector<SkeletalVertex>& boneData)
// {
//   for(unsigned int i = 0; i < boneData.size(); i++)
//   {
//     glm::vec4& boneWeights = boneData[i].boneWeights;
//     float total = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
//     if(total > 0.f) {
//       boneData[i].boneWeights = glm::vec4(
//         boneWeights.x / total,
//         boneWeights.y / total,
//         boneWeights.z / total,
//         boneWeights.w / total
//       );
//     }
//     // else {
//     //    std::cout << "err: no weights" << i << std::endl;
//     // }
//
//     // if(total > 1) std::cout << "info:: normalized" << std::endl;
//   }
// }

void SkeletalModel::addBone(const aiBone* aiBone, BoneInfos& outBoneInfos) const
{
  const std::string name = aiBone->mName.C_Str();
  if (outBoneInfos.find(name) == outBoneInfos.end()) {
    const unsigned int index = outBoneInfos.size();
    outBoneInfos[name] = BoneInfo {
      .index = index,
      .offset = Assimp2glm::mat4(aiBone->mOffsetMatrix),
    };
  }
}

bool SkeletalModel::readSkeleton(const aiNode* node, const BoneInfos& boneInfos, Bone& skeleton)
  const
{
  const char* name = node->mName.C_Str();
  if (boneInfos.find(name) != boneInfos.end()) {
    const BoneInfo& info = boneInfos.at(node->mName.C_Str());
    skeleton.name = name;
    skeleton.index = info.index;
    skeleton.offset = info.offset;

#ifdef WITH_TRANSFORM
    std::cout << skeleton.name << "\n";
    skeleton.transform = Assimp2glm::mat4(node->mTransformation);
#endif
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      Bone child;
      if (readSkeleton(node->mChildren[i], boneInfos, child)) {
        skeleton.children.push_back(child);
      }
    }
    return true;
  } else {
    // std::cout << "info: Seaching children:" << node->mName.C_Str() << std::endl;
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      if (readSkeleton(node->mChildren[i], boneInfos, skeleton)) {
        return true;
      }
    }
  }
  return false;
}

// ANIMATOR - move?

// void printMat4(glm::mat4 mat4)
// {
//   for(unsigned int i = 0; i < 4; i++)
//   {
//     for(unsigned int j = 0; j < 4; j++)
//     {
//       std::cout << mat4[i][j];
//     }
//   }
// }

unsigned int SkeletalModel::addAnimation(Animation animation)
{
  mAnimations.push_back(animation);

  return mAnimations.size() - 1;
}
