#ifndef SKELETAL_MESH
#define SKELETAL_MESH

#include "mesh.h"
#include "skeletal-vertex.h"
#include <vector>

class SkeletalMesh
{
public:
  SkeletalMesh(const unsigned int vao, const std::vector<SkeletalVertex>& vertices)
  {
    setupBones(vao, vertices);
  }
  ~SkeletalMesh() { }

  void setupBones(const unsigned int vao, const std::vector<SkeletalVertex>& vertices);
  void free();

private:
  unsigned int mBufferId;
};

#endif
