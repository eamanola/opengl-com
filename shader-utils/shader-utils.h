#ifndef SHADER_UTILS_h
#define SHADER_UTILS_h

#include <glad/gl.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../scene.h"

class ShaderUtils
{
  public:
    static const unsigned int compileShader(
      GLenum type, const char* path,
      const std::vector<std::string>& prependTexts = {},
      const std::vector<std::string>& prependFiles = {}
    );
    static unsigned int loadTexture(const char* path, const GLint wrap = GL_REPEAT);
    static unsigned int loadTexture(const unsigned char* buffer, unsigned int len, const GLint wrap = GL_REPEAT);
    static bool createFramebufferTexture2D(
      const float width, const float height,
      unsigned int &outFBO, unsigned int &outTextureId, unsigned int &outRBO
    );
    void static screenshot(
      Scene &scene, const unsigned int& FBO, const glm::vec3& position, const glm::vec3& normal
    );
    // paths order:
    // GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Back
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Front
    static unsigned int loadCubemap(std::vector<std::string> paths);

  private:
    static const std::string readFile(const char* path);
    static const unsigned int compile(GLenum type, const char* source);
    static const unsigned int createTexture(int width, int height, int nrChannel, unsigned char* data, const GLint wrap = GL_REPEAT);
};

#endif
