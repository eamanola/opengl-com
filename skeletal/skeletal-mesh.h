#ifndef SKELETAL_MESH
#define SKELETAL_MESH

#include "../mesh.h"
#include "skeletal-vertex.h"
#include <vector>

class SkeletalMesh
{
public:
  SkeletalMesh(unsigned int VAO, const std::vector<SkeletalVertex> &vertices);
  ~SkeletalMesh();

  void free();

private:
  void setupBones(unsigned int VAO, const std::vector<SkeletalVertex> &vertices);
  unsigned int VBO;
};


#endif
