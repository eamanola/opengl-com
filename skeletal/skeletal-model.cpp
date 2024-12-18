#include "skeletal-model.h"
#include "assimp_glm_helpers.h"
#include <glm/gtx/quaternion.hpp>

SkeletalModel::SkeletalModel()
:
mAnimationIndex(0)
{
}

SkeletalModel::~SkeletalModel()
{
}

void SkeletalModel::processScene(const aiScene* scene)
{
  Model::processScene(scene);

  if(scene->mNumAnimations == 0)
  {
    std::cout << "err: no animations available" << std::endl;
    return;
  }

  for(unsigned int i = 0; i < scene->mNumAnimations; i++)
  {

    aiAnimation* anim = scene->mAnimations[i];
    std::cout << "info:" << i << " " << anim->mName.C_Str() << std::endl;

    addAnimation(readAnimation(anim));
  }

  for(unsigned int i = 0; i < scene->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[i];
    // std::cout << mesh->mName.C_Str() << std::endl;

    std::vector<SkeletalVertex> boneData = readBoneData(mesh);

    mMeshes.push_back(SkeletalMesh(meshes[i].vao(), boneData));
  }

  readSkeleton(scene->mRootNode, mRootBone);

  mCurrentPose.resize(mBoneInfoMap.size(), glm::mat4(1.0));

  // for (const std::pair<const std::string, BoneInfo>& n : mBoneInfoMap)
  //   std::cout << n.first << "\n"; // , n.second);
}

Animation SkeletalModel::readAnimation(const aiAnimation* anim)
{
  Animation animation {
    .duration = (float)anim->mDuration,
    .ticksPerSecond = anim->mTicksPerSecond != 0.f ? (float)anim->mTicksPerSecond : 1.f,
    .boneTransforms = {}
  };

  for(unsigned int i = 0; i < anim->mNumChannels; i++)
  {
    const aiNodeAnim* channel = anim->mChannels[i];
    const std::string boneName = channel->mNodeName.C_Str();
    BoneTransforms transforms;

    for(unsigned int j = 0; j < channel->mNumPositionKeys; j++)
    {
      transforms.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
      transforms.positions.push_back(AssimpGLMHelpers::GetGLMVec(channel->mPositionKeys[j].mValue));
    }

    for(unsigned j = 0; j < channel->mNumRotationKeys; j++)
    {
      transforms.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
      transforms.rotations.push_back(AssimpGLMHelpers::GetGLMQuat(channel->mRotationKeys[j].mValue));
    }

    for(unsigned j = 0; j < channel->mNumScalingKeys; j++)
    {
      transforms.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
      transforms.scales.push_back(AssimpGLMHelpers::GetGLMVec(channel->mScalingKeys[j].mValue));
    }

    animation.boneTransforms[boneName] = transforms;
  }

  return animation;
}

std::vector<SkeletalVertex> SkeletalModel::readBoneData(const aiMesh* mesh)
{
  std::vector<SkeletalVertex> boneData(mesh->mNumVertices, SkeletalVertex {});

  for(unsigned int i = 0; i < mesh->mNumBones; i++)
  {
    const aiBone* bone = mesh->mBones[i];

    const BoneInfo* boneInfo = addBone(
      bone->mName.C_Str(),
      AssimpGLMHelpers::ConvertMatrixToGLMFormat(bone->mOffsetMatrix)
    );

    for(unsigned int j = 0; j < bone->mNumWeights; j++)
    {
      unsigned int vertexId = bone->mWeights[j].mVertexId;
      float weight = bone->mWeights[j].mWeight;

      // has no effect
      if(weight == 0.f)
      {
        continue;
      }

      unsigned int bwi = 0;
      for(; bwi < MAX_BONE_INFLUENCE; bwi++)
      {
        if(boneData[vertexId].boneWeights[bwi] == 0.f)
        {
          boneData[vertexId].boneIds[bwi] = boneInfo->index;
          boneData[vertexId].boneWeights[bwi] = weight;
          break;
        }
      }

      if(bwi == MAX_BONE_INFLUENCE)
      {
        replaceOrDiscard(boneInfo->index, weight, boneData[vertexId]);
        // std::cout << "info: too many bones v: " << vertexId;
        // if(replaceOrDiscard(outBoneData[vertexId], boneInfo.index, weight))
        // {
        //   std::cout << " replaced old" << std::endl;
        // }
        // else
        // {
        //   std::cout << " discarded new" << std::endl;
        // }
      }
    }
  }

  // should come normalized
  // normalizeWeights(boneData);

  return boneData;
}

bool SkeletalModel::replaceOrDiscard(
  unsigned int boneId, float weight, SkeletalVertex &outVertexBoneData
)
{
  float* minWeight = std::min_element(
    std::begin(outVertexBoneData.boneWeights), std::end(outVertexBoneData.boneWeights)
  );

  if(*minWeight < weight)
  {
    unsigned int minWeightIndex = (unsigned int)std::distance(
      std::begin(outVertexBoneData.boneWeights), minWeight
    );

    outVertexBoneData.boneIds[minWeightIndex] = boneId;
    outVertexBoneData.boneWeights[minWeightIndex] = weight;

    return true;
  }

  return false;
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

const BoneInfo* SkeletalModel::addBone(const std::string &name, glm::mat4 offset)
{
  if(mBoneInfoMap.find(name) == mBoneInfoMap.end())
  {
    const unsigned int index = mBoneInfoMap.size();
    mBoneInfoMap[name] = BoneInfo { .index = index, .offset = offset };
  }

  return &mBoneInfoMap[name];
}

bool SkeletalModel::readSkeleton(const aiNode* node, Bone& skeleton)
{
  if(mBoneInfoMap.find(node->mName.C_Str()) != mBoneInfoMap.end())
  {
    skeleton.name = node->mName.C_Str();

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
      Bone child;
      readSkeleton(node->mChildren[i], child);
      skeleton.children.push_back(child);
    }
    return true;
  }
  else
  {
    // std::cout << "info: Seaching children:" << node->mName.C_Str() << std::endl;
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
      if(readSkeleton(node->mChildren[i], skeleton)) {
        return true;
      }
    }
  }
  return false;
}

std::pair<int, float> SkeletalModel::getTimeFraction(
  const std::vector<float>& times, float animTime
) const
{
  unsigned int segment = 0;
  while (animTime >= times[segment]) segment ++;

  float start = times[segment - 1];
  float end = times[segment];
  float frac = (animTime - start) / (end - start);

  return { segment, frac };
}

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

void SkeletalModel::updatePose(
  const Animation& animation,
  const Bone& bone,
  const float& timeInSec,
  const glm::mat4& parentTransform,
  std::vector<glm::mat4>& output
)
{
  const BoneTransforms* transforms;
  if(animation.boneTransforms.find(bone.name) != animation.boneTransforms.end())
  {
    transforms = &animation.boneTransforms.at(bone.name);
  }
  else
  {
    BoneTransforms empty;
    transforms = &empty;
  }

  const glm::mat4 IDENTITY = glm::mat4(1.f);

  const BoneInfo& boneInfo = mBoneInfoMap[bone.name];
  const float animationTime = fmod(timeInSec * animation.ticksPerSecond, animation.duration);

  //local transform
  glm::mat4 positionMat = IDENTITY;
  glm::mat4 rotationMat = IDENTITY;
  glm::mat4 scaleMat = IDENTITY;

  const unsigned int posSize = transforms->positions.size();
  if(posSize == 1)
  {
    // check timestamp?
    positionMat = glm::translate(IDENTITY, transforms->positions[0]);
  }
  else if(posSize > 1)
  {
    const std::pair<unsigned int, float> fp = getTimeFraction(transforms->positionTimestamps, animationTime);
    glm::vec3 position1 = transforms->positions[fp.first - 1];
    glm::vec3 position2 = transforms->positions[fp.first];
    glm::vec3 position = glm::mix(position1, position2, fp.second);

    positionMat = glm::translate(IDENTITY, position);
  }

  const unsigned int rotSize = transforms->rotations.size();
  if(rotSize == 1)
  {
    rotationMat = glm::toMat4(glm::normalize(transforms->rotations[0]));
  }
  else if(rotSize > 1)
  {
    const std::pair<unsigned int, float> fp = getTimeFraction(transforms->rotationTimestamps, animationTime);
    glm::quat rotation1 = transforms->rotations[fp.first - 1];
    glm::quat rotation2 = transforms->rotations[fp.first];
    glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);
    rotationMat = glm::toMat4(glm::normalize(rotation));
  }

  const unsigned int scaSize = transforms->scales.size();
  if(scaSize == 1)
  {
    scaleMat = glm::scale(IDENTITY, transforms->scales[0]);
  }
  else if(scaSize > 1)
  {
    const std::pair<unsigned int, float> fp = getTimeFraction(transforms->scaleTimestamps, animationTime);
    glm::vec3 scale1 = transforms->scales[fp.first - 1];
    glm::vec3 scale2 = transforms->scales[fp.first];
    glm::vec3 scale = glm::mix(scale1, scale2, fp.second);
    scaleMat = glm::scale(IDENTITY, scale);
  }

  glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
  glm::mat4 globalTransform = parentTransform * localTransform;

  if(globalTransform == IDENTITY) output[boneInfo.index] = IDENTITY;
  else output[boneInfo.index] = /*globalInverseTransform * */globalTransform * boneInfo.offset;

  for(unsigned int i = 0; i < bone.children.size(); i++)
  {
    updatePose(animation, bone.children[i], timeInSec, globalTransform, output);
  }
}

void SkeletalModel::update(float timeInMSec)
{
  glm::mat4 identity = glm::mat4(1.0f);
  updatePose(mAnimations[mAnimationIndex], mRootBone, timeInMSec, identity, mCurrentPose);
}

const std::vector<glm::mat4> SkeletalModel::pose() const
{
  return mCurrentPose;
}

const Animation* SkeletalModel::setAnimation(const unsigned int animationIndex)
{
  if(animationIndex < mAnimations.size())
  {
    mAnimationIndex = animationIndex;
    return &mAnimations[animationIndex];
  }

  return nullptr;
}

unsigned int SkeletalModel::addAnimation(Animation animation)
{
  mAnimations.push_back(animation);

  return mAnimations.size() - 1;
}

void SkeletalModel::free()
{
  for(unsigned int i = 0; i < mMeshes.size(); i ++)
  {
    mMeshes[i].free();
  }

  Model::free();
}
