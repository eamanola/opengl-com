#ifndef MIRROR_H
#define MIRROR_H

#include "renderable.h"

#include "mesh.h"
#include "scene.h"
#include "texture.h"
#include <glm/glm.hpp>

class Mirror : public Renderable
{
public:
  Mirror(const float vWidth = 800.f, const float vHeight = 600.f);
  ~Mirror() { }

  void screenshot(Scene& scene, const glm::vec3& positionOffset);
  void screenshot(Scene& scene);

  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

private:
  glm::vec3 mNormal;
  unsigned int mFBO;
  unsigned int mRBO;
  Mesh mMesh;
  Texture mTexture;
  const float mWidth;
  const float mHeight;

  unsigned int mFBOI;
  unsigned int mTexI;
};

#endif
