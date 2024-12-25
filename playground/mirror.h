#ifndef MIRROR_H
#define MIRROR_H

#include "../drawable.h"
#include "../scene.h"
#include <glm/glm.hpp>
#include "../mesh.h"

class Mirror : Drawable
{
public:
  Mirror(const float vWidth = 800.f, const float vHeight = 600.f);
  ~Mirror();

  void screenshot(Scene &scene);

  void update(const float& time) override {};
  void draw(const Shader& shader) override ;
  void free() override ;

private:
  void setupFrameBuffer(const float vWidth, const float vHeight);

  glm::vec3 mPosition;
  glm::vec3 mNormal;
  unsigned int mFBO;
  unsigned int mRBO;
  Mesh mMesh;
  Texture mTexture;
};


#endif
