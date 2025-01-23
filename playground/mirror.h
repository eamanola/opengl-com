#ifndef MIRROR_H
#define MIRROR_H

#include "renderable.h"

#include "mesh.h"
#include "render-buffer.h"
#include "scene.h"
#include "texture.h"
#include <glm/glm.hpp>

class Mirror : public Renderable
{
public:
  Mirror(const float width, const float height, const unsigned int samples);
  ~Mirror() { }

  void screenshot(const Scene& scene, const glm::vec3& positionOffset) const;
  void screenshot(const Scene& scene) const;

  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

private:
  const glm::vec3 mNormal;
  const Mesh mMesh;
  const RenderBuffer mRenderBuffer;
};

#endif
