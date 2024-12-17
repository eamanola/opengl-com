#include "skeletal-mesh.h"

SkeletalMesh::SkeletalMesh(unsigned int VAO, const std::vector<SkeletalVertex> &vertices)
{
  setupBones(VAO, vertices);
}

SkeletalMesh::~SkeletalMesh()
{
}

void SkeletalMesh::setupBones(unsigned int VAO, const std::vector<SkeletalVertex> &vertices)
{
  // for(unsigned int i = 0; i < vertices.size(); i ++)
  // {
  //   vertices[i] = SkeletalVertex {
  //     // .boneIds =  glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
  //     .boneWeights =  glm::vec4(1.0f)
  //   };
  // }
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(
    GL_ARRAY_BUFFER, vertices.size() * sizeof(SkeletalVertex), &vertices[0], GL_STATIC_DRAW
  );

  glVertexAttribPointer(
    3, MAX_BONE_INFLUENCE, GL_UNSIGNED_INT, GL_FALSE, sizeof(SkeletalVertex),
    (void*)(offsetof(SkeletalVertex, boneIds))
  );
  glEnableVertexAttribArray(3);

  glVertexAttribPointer(
    4, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(SkeletalVertex),
    (void*)(offsetof(SkeletalVertex, boneWeights))
  );
  glEnableVertexAttribArray(4);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void SkeletalMesh::free()
{
  glDeleteBuffers(1, &VBO);
}
