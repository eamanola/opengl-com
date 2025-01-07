#ifndef SKELETAL_MESH
#define SKELETAL_MESH

#include "mesh.h"
#include "skeletal-vertex.h"
#include <vector>

namespace SkeletalMesh
{

unsigned int setupBones(Mesh& mesh, const std::vector<SkeletalVertex>& vertices);

} // namespace SkeletalMesh

#endif
