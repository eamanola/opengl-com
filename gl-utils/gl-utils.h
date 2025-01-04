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
    static bool compileShader(const GLenum type, const char* source, unsigned int& shaderId);
    static bool createTexture2D(const int width, const int height, const int nrChannel, const unsigned char* data, unsigned int& textureId, const GLint wrap = GL_REPEAT);
    static bool deleteTextures(const unsigned int lenght, const unsigned int* textureId);
    static std::vector<int> checkErrors();
    static bool noErrors();
};

#endif
