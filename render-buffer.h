#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "texture.h"
#include <glad/gl.h>
#include <vector>

class RenderBuffer
{
public:
  enum Format {
    RGB = GL_RGB,
    RGB16F = GL_RGB16F,
    RGBA16F = GL_RGBA16F,
  };

  // upto 8 color outputs guaranteed
  // can query max with GL_MAX_COLOR_ATTACHMENTS
  RenderBuffer(
    unsigned int samples,
    Format internal,
    const std::size_t width,
    const std::size_t height,
    unsigned int colorOutputs
  );
  ~RenderBuffer() { }
  void free() const;
  void blit() const;

  const unsigned int& fbo() const { return mFBO; }
  const Texture* textures(unsigned int index = 0) const { return &mTextures[index]; }

private:
  unsigned int mFBO;
  unsigned int mRBO;

  std::vector<Texture> mTextures;
  const std::size_t mWidth;
  const std::size_t mHeight;

  unsigned int mFBOI;
  std::vector<unsigned int> mTexI;
};

#endif
