#include "render-buffer.h"

#include "gl-utils/gl-utils.h"
#include <cassert>

RenderBuffer::RenderBuffer(
  unsigned int samples, Format internal, const std::size_t width, const std::size_t height
) :
  mTexture(Texture { .type = TEXTURE_TYPE_DIFFUSE }), mWidth(width), mHeight(height)
{
  GLenum format;
  GLenum type;

  switch (internal) {
  case RGB:
    format = GL_RGB;
    type = GL_UNSIGNED_BYTE;
    break;

  case RGBA16F:
    format = GL_RGBA;
    type = GL_FLOAT;
    break;

  default:
    assert(false);
    break;
  }

  if (samples > 1) {
    // multisampled framebuffer
    // no output format&type in multisample
    GLUtils::Framebuffer::createFramebufferTexture2D(
      mFBO, &mTexI, &mRBO, samples, internal, width, height, GL_NONE, GL_NONE
    );

    // framebuffer to downscale/relove to, for drawing
    GLUtils::Framebuffer::createFramebufferTexture2D(
      mFBOI, &mTexture.id, nullptr, 1, internal, width, height, format, type
    );
  } else {
    GLUtils::Framebuffer::createFramebufferTexture2D(
      mFBO, &mTexture.id, &mRBO, 1, internal, width, height, format, type
    );

    mFBOI = 0;
    mTexI = 0;
  }
}

void RenderBuffer::blit() const
{
  // resolve framebuffer
  if (mFBOI != 0) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
    // glReadBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBOI);
    // glDrawBuffer(GL_COLOR_ATTACHMENT0);

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
