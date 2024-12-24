#ifndef MIRROR_H
#define MIRROR_H

#include "../scene.h"
#include <glm/glm.hpp>
#include "../mesh.h"

class Mirror
{
public:
  Mirror(const float vWidth = 800.f, const float vHeight = 600.f);
  ~Mirror();

  void update(Scene &scene);
  void draw(Shader& shader);
  void free();

private:
  void setupFrameBuffer(const float vWidth, const float vHeight);

  glm::vec3 mPosition;
  glm::vec3 mNormal;
  unsigned int mFBO;
  unsigned int mRBO;
  Mesh mMesh;
  Texture mTexture;

  glm::mat4 model();
};


#endif
