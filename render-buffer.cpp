#include "render-buffer.h"

#include "gl-utils/gl-utils.h"
#include "utils/utils.h"
#include <cassert>
#include <iostream>

RenderBuffer::RenderBuffer(
  unsigned int samples,
  Format internal,
  const std::size_t width,
  const std::size_t height,
  unsigned int colorOutputs
) :
  mTextures(colorOutputs, Texture { .type = TEXTURE_TYPE_DIFFUSE }), mWidth(width), mHeight(height)
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
    unsigned int texI[colorOutputs] = {};
    unsigned int tex[colorOutputs] = {};
    // multisampled framebuffer
    // no output format&type in multisample
    GLUtils::Framebuffer::createFramebufferTexture2D(
      mFBO, texI, colorOutputs, &mRBO, samples, internal, width, height, GL_NONE, GL_NONE
    );

    // framebuffer to downscale/relove to, for drawing
    GLUtils::Framebuffer::createFramebufferTexture2D(
      mFBOI, tex, colorOutputs, nullptr, 1, internal, width, height, format, type
    );

    mTexI.reserve(colorOutputs);
    for (unsigned int i = 0; i < colorOutputs; i++) {
      mTextures[i].id = tex[i];
      mTexI.push_back(texI[i]);
    }
  } else {
    unsigned int tex[colorOutputs] = {};

    GLUtils::Framebuffer::createFramebufferTexture2D(
      mFBO, tex, colorOutputs, &mRBO, 1, internal, width, height, format, type
    );

    for (unsigned int i = 0; i < colorOutputs; i++) {
      mTextures[i].id = tex[i];
    }

    mFBOI = 0;
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
  Utils::Textures::deleteTextures(mTextures);
  glDeleteRenderbuffers(1, &mRBO);

  if (mFBOI != 0) {
    glDeleteFramebuffers(1, &mFBOI);
    glDeleteTextures(mTexI.size(), &mTexI[0]);
  }
}
