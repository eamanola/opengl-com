#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "texture.h"
#include <glad/gl.h>

class RenderBuffer
{
public:
  enum Format {
    RGB = GL_RGB,
    RGBA16F = GL_RGBA16F,
  };

  RenderBuffer(
    unsigned int samples, Format internal, const std::size_t width, const std::size_t height
  );
  ~RenderBuffer() { }
  void free() const;
  void blit() const;

  const unsigned int& fbo() const { return mFBO; }
  const Texture& texture() const { return mTexture; }

private:
  unsigned int mFBO;
  unsigned int mRBO;

  Texture mTexture;
  const std::size_t mWidth;
  const std::size_t mHeight;

  unsigned int mFBOI;
  unsigned int mTexI;
};

#endif
