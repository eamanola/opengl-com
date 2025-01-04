#ifndef MIRROR_H
#define MIRROR_H

#include "drawable.h"
#include "scene.h"
#include <glm/glm.hpp>
#include "mesh.h"

class Mirror : public Drawable
{
public:
  Mirror(const float vWidth = 800.f, const float vHeight = 600.f);
  ~Mirror();

  void screenshot(Scene &scene, const glm::vec3& positionOffset);
  void screenshot(Scene &scene);

  void update(const float& time) override {};
  void draw(const Shader& shader) const override;
  void free() const override;

private:
  glm::vec3 mNormal;
  unsigned int mFBO;
  unsigned int mRBO;
  Mesh mMesh;
  Texture mTexture;
};


#endif
