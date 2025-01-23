#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "texture.h"

class RenderBuffer
{
public:
  RenderBuffer(unsigned int width, unsigned int height, unsigned int samples);
  ~RenderBuffer() { }
  void free() const;
  void blit() const;

  const unsigned int& fbo() const { return mFBO; }
  const Texture& texture() const { return mTexture; }

private:
  unsigned int mFBO;
  unsigned int mRBO;

  Texture mTexture;
  const float mWidth;
  const float mHeight;

  unsigned int mFBOI;
  unsigned int mTexI;
};

#endif
