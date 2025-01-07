#include "skeletal-mesh.h"
#include "shaders/attrib-locations.h"

unsigned int SkeletalMesh::setupBones(Mesh& mesh, const std::vector<SkeletalVertex>& vertices)
{
  std::vector<Mesh::VertexAttribPointer> attribPointers;

  Mesh::VertexAttribPointer boneIds = {
    .location = ATTRIB_LOCATIONS::BONE_IDS,
    .size = MAX_BONE_INFLUENCE,
    .type = Mesh::AttribType::UNSIGNED_INT,
    .stride = sizeof(SkeletalVertex),
    .offset = (void*)(offsetof(SkeletalVertex, boneIds)),
  };
  attribPointers.push_back(boneIds);

  Mesh::VertexAttribPointer boneWeights = {
    .location = ATTRIB_LOCATIONS::BONE_WEIGHTS,
    .size = MAX_BONE_INFLUENCE,
    .stride = sizeof(SkeletalVertex),
    .offset = (void*)(offsetof(SkeletalVertex, boneWeights)),
  };
  attribPointers.push_back(boneWeights);

  unsigned int VBO;
  mesh.addBuffer(VBO, &vertices[0], sizeof(SkeletalVertex) * vertices.size(), attribPointers);

  return VBO;
}
