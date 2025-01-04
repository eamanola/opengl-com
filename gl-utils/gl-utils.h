#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <glad/gl.h>
#include <string>
#include <vector>

class GLUtils
{
  public:
    static bool createFramebufferTexture2D(
      const float width, const float height,
      unsigned int &outFBO, unsigned int &outTextureId, unsigned int &outRBO
    );
    // paths order:
    // GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Back
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Front
    static unsigned int loadCubemap(std::vector<std::string> paths);
    static const unsigned int compileShader(GLenum type, const char* source);
    static const unsigned int createTexture2D(int width, int height, int nrChannel, unsigned char* data, const GLint wrap = GL_REPEAT);
    static void deleteTextures(const unsigned int lenght, const unsigned int* textureId);

  private:
};

#endif
