#ifndef UTILS_H
#define UTILS_H

#include "scene.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glad/gl.h>
#include "texture.h"

class Utils
{
public:
  Utils() {}
  ~Utils() {}

  static void screenshot(
    Scene &scene, const unsigned& FBO, const glm::vec3& position, const glm::vec3& normal
  );

  static std::string readFile(const char* path);

  static std::string shaderSource(
    const char* path,
    const std::vector<std::string>& prependTexts,
    const std::vector<std::string>& prependFiles
  );

  static Texture loadTexture2D(const char* path, TEXTURE_TYPE type);
  static unsigned int loadTexture2D(const char* path, const int wrap = GL_REPEAT);
  static unsigned int loadTexture2D(const unsigned char* buffer, const unsigned int len, const int wrap = GL_REPEAT);
  static void deleteTextures(const std::vector<Texture>& textures);
private:
  static unsigned int createTexture2D(const int width, const int height, const int nrChannel, const unsigned char* data, const int wrap);
};

#endif
