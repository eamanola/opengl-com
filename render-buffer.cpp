#include "render-buffer.h"

#include "gl-utils/gl-utils.h"

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height, unsigned int samples) :
  mTexture(Texture { .type = TEXTURE_TYPE_DIFFUSE }), mWidth(width), mHeight(height)
{
  if (samples > 1) {
    // multisampled framebuffer
    GLUtils::Framebuffer::createFramebufferTexture2D(width, height, samples, mFBO, &mTexI, &mRBO);

    // framebuffer to downscale/relove to, for drawing
    GLUtils::Framebuffer::createFramebufferTexture2D(
      width, height, 1, mFBOI, &mTexture.id, nullptr
    );
  } else {
    GLUtils::Framebuffer::createFramebufferTexture2D(width, height, 1, mFBO, &mTexture.id, &mRBO);

    mFBOI = 0;
    mTexI = 0;
  }
}

void RenderBuffer::blit() const
{
  // resolve framebuffer
  if (mFBOI != 0) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBOI);
    glBlitFramebuffer(
      0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // GL_READ_FRAMEBUFFER + GL_DRAW_FRAMEBUFFER
  }
}

void RenderBuffer::free() const
{
  glDeleteFramebuffers(1, &mFBO);
  glDeleteTextures(1, &mTexture.id);
  glDeleteRenderbuffers(1, &mRBO);

  if (mFBOI != 0) {
    glDeleteFramebuffers(1, &mFBOI);
    glDeleteTextures(1, &mTexI);
  }
}
