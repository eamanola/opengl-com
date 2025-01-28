#ifndef G_BUFFER_H
#define G_BUFFER_H

#include "texture.h"
#include <vector>

class GBuffer
{
public:
  GBuffer(const std::size_t width, const std::size_t height);
  ~GBuffer() { }

  void free() const;
  const Texture* textures(unsigned int index = 0) const { return &mTextures[index]; }
  const unsigned int count() const { return mTextures.size(); }
  const unsigned int& fbo() const { return mFbo; }

private:
  std::vector<Texture> mTextures;
  unsigned int mFbo;
  unsigned int mRbo;
  /* data */
};

#endif
