#include "skeletal-mesh.h"

#include "gl-utils/gl-utils.h"
#include "shaders/attrib-locations.h"

void SkeletalMesh::setupBones(const unsigned int vao, const std::vector<SkeletalVertex>& vertices)
{
  std::vector<GLUtils::VertexAttribPointer> attribPointers;

  GLUtils::VertexAttribPointer boneIds = {
    .location = ATTRIB_LOCATIONS::BONE_IDS,
    .size = MAX_BONE_INFLUENCE,
    .type = GLUtils::AttribType::UNSIGNED_INT,
    .stride = sizeof(SkeletalVertex),
    .offset = (void*)(offsetof(SkeletalVertex, boneIds)),
  };
  attribPointers.push_back(boneIds);

  GLUtils::VertexAttribPointer boneWeights = {
    .location = ATTRIB_LOCATIONS::BONE_WEIGHTS,
    .size = MAX_BONE_INFLUENCE,
    .stride = sizeof(SkeletalVertex),
    .offset = (void*)(offsetof(SkeletalVertex, boneWeights)),
  };
  attribPointers.push_back(boneWeights);

  GLUtils::addVertexBuffer(
    mBufferId, vao, &vertices[0], sizeof(SkeletalVertex) * vertices.size(), attribPointers
  );
}

void SkeletalMesh::free() { glDeleteBuffers(1, &mBufferId); }
