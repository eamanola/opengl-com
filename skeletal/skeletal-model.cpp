#include "skeletal-model.h"
#include "assimp_glm_helpers.h"
#include <glm/gtx/quaternion.hpp>

SkeletalModel::SkeletalModel()
:
mAnimationIndex(0),
mAnimations({}),
mMeshes({}),
mBoneInfoMap({}),
mRootBone({}),
mCurrentPose({})
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
    Animation animation;
    aiAnimation* anim = scene->mAnimations[i];
    std::cout << "info:" << i << " " << anim->mName.C_Str() << std::endl;

    readAnimation(anim, animation);

    mAnimations.push_back(animation);
  }

  for(unsigned int i = 0; i < scene->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[i];
    // std::cout << mesh->mName.C_Str() << std::endl;

    std::vector<SkeletalVertex> boneData(
      mesh->mNumVertices,
      SkeletalVertex { .boneIds = {}, .boneWeights = {} }
    );

    readBoneData(mesh, boneData, mBoneInfoMap);

    mMeshes.push_back(SkeletalMesh(meshes[i].VAO, boneData));
  }

  readSkeleton(scene->mRootNode, mRootBone);

  mCurrentPose.resize(mBoneInfoMap.size(), glm::mat4(1.0));
}

void SkeletalModel::readAnimation(const aiAnimation* anim, Animation& outAnimation)
{
  if(anim->mTicksPerSecond != 0.f)
  {
    outAnimation.ticksPerSecond = anim->mTicksPerSecond;
  }

  outAnimation.duration = anim->mDuration;

  outAnimation.boneTransforms = {};
  for(unsigned int i = 0; i < anim->mNumChannels; i++)
  {
    aiNodeAnim* channel = anim->mChannels[i];
    std::string boneName = channel->mNodeName.C_Str();
    BoneTransform transforms;

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

    outAnimation.boneTransforms[boneName] = transforms;
  }
}

void SkeletalModel::readBoneData(
  const aiMesh* mesh,
  std::vector<SkeletalVertex>& outBoneData,
  std::unordered_map<std::string, BoneInfo>& outBoneInfoMap
)
{
  for(unsigned int i = 0; i < mesh->mNumBones; i++)
  {
    aiBone* bone = mesh->mBones[i];

    BoneInfo boneInfo = addBone(
      bone->mName.C_Str(),
      AssimpGLMHelpers::ConvertMatrixToGLMFormat(bone->mOffsetMatrix),
      outBoneInfoMap
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
        if(outBoneData[vertexId].boneWeights[bwi] == 0.f)
        {
          outBoneData[vertexId].boneIds[bwi] = boneInfo.index;
          outBoneData[vertexId].boneWeights[bwi] = weight;
          break;
        }
      }

      if(bwi == MAX_BONE_INFLUENCE)
      {
        replaceOrDiscard(boneInfo.index, weight, outBoneData[vertexId]);
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
}

bool SkeletalModel::replaceOrDiscard(
  const unsigned int boneId, const float weight, SkeletalVertex &outVertexBoneData
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

BoneInfo SkeletalModel::addBone(
  const std::string name, const glm::mat4 offset,
  std::unordered_map<std::string, BoneInfo>& outBoneInfoMap
)
{
  if(outBoneInfoMap.find(name) == outBoneInfoMap.end())
  {
    const unsigned int index = outBoneInfoMap.size();
    outBoneInfoMap[name] = BoneInfo { .index = index, .offset = offset };
  }

  return outBoneInfoMap[name];
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
  const std::vector<float>& times, const float& animTime
) const
{
  unsigned int segment = 0;
  while (animTime > times[segment]) segment ++;

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

void SkeletalModel::getPose(
  Animation& animation,
  const Bone& bone,
  const float timeInSec,
  const glm::mat4& parentTransform,
  std::vector<glm::mat4>& output
)
{
  const glm::mat4 IDENTITY = glm::mat4(1.f);
  const BoneTransform& bt = animation.boneTransforms[bone.name];
  const BoneInfo& boneInfo = mBoneInfoMap[bone.name];
  const float animationTime = fmod(timeInSec, animation.duration);

  //local transform
  glm::mat4 positionMat = IDENTITY;
  glm::mat4 rotationMat = IDENTITY;
  glm::mat4 scaleMat = IDENTITY;

  const unsigned int posSize = bt.positions.size();
  if(posSize == 1)
  {
    // check timestamp?
    positionMat = glm::translate(IDENTITY, bt.positions[0]);
  }
  else if(posSize > 1)
  {
    const std::pair<unsigned int, float> fp = getTimeFraction(bt.positionTimestamps, animationTime);
    glm::vec3 position1 = bt.positions[fp.first - 1];
    glm::vec3 position2 = bt.positions[fp.first];
    glm::vec3 position = glm::mix(position1, position2, fp.second);

    positionMat = glm::translate(IDENTITY, position);
  }

  const unsigned int rotSize = bt.rotations.size();
  if(rotSize == 1)
  {
    rotationMat = glm::toMat4(glm::normalize(bt.rotations[0]));
  }
  else if(rotSize > 1)
  {
    const std::pair<unsigned int, float> fp = getTimeFraction(bt.rotationTimestamps, animationTime);
    glm::quat rotation1 = bt.rotations[fp.first - 1];
    glm::quat rotation2 = bt.rotations[fp.first];
    glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);
    rotationMat = glm::toMat4(glm::normalize(rotation));
  }

  const unsigned int scaSize = bt.scales.size();
  if(scaSize == 1)
  {
    scaleMat = glm::scale(IDENTITY, bt.scales[0]);
  }
  else if(scaSize > 1)
  {
    const std::pair<unsigned int, float> fp = getTimeFraction(bt.scaleTimestamps, animationTime);
    glm::vec3 scale1 = bt.scales[fp.first - 1];
    glm::vec3 scale2 = bt.scales[fp.first];
    glm::vec3 scale = glm::mix(scale1, scale2, fp.second);
    scaleMat = glm::scale(IDENTITY, scale);
  }

  glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
  glm::mat4 globalTransform = parentTransform * localTransform;

  output[boneInfo.index] = /*globalInverseTransform * */globalTransform * boneInfo.offset;

  for(unsigned int i = 0; i < bone.children.size(); i++)
  {
    getPose(
      animation,
      bone.children[i],
      timeInSec,
      globalTransform,
      output
    );
  }
}

void SkeletalModel::update(float timeInMSec)
{
  glm::mat4 identity = glm::mat4(1.0f);
  getPose(mAnimations[mAnimationIndex], mRootBone, timeInMSec * 1000, identity, mCurrentPose);
}

const std::vector<glm::mat4> SkeletalModel::pose() const
{
  return mCurrentPose;
}

bool SkeletalModel::setAnimation(const unsigned int animationIndex)
{
  if(animationIndex < mAnimations.size())
  {
    mAnimationIndex = animationIndex;
    return true;
  }

  return false;
}

void SkeletalModel::free()
{
  for(unsigned int i = 0; i < mMeshes.size(); i ++)
  {
    mMeshes[i].free();
  }

  Model::free();
}
