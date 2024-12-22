#ifndef MIRROR_H
#define MIRROR_H

#include "../scene.h"
#include <glm/glm.hpp>
#include "../mesh.h"

class Mirror
{
public:
  Mirror(/* args */);
  ~Mirror();

  void update(Scene &scene);
  void draw(Shader& shader);
  void free();

private:
  void setupFrameBuffer();

  glm::vec3 mPosition;
  glm::vec3 mNormal;
  unsigned int mFBO;
  unsigned int mRBO;
  Mesh mMesh;
  Texture mTexture;

  glm::mat4 model();
};


#endif
