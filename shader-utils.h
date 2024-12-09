#ifndef SHADER_UTILS_h
#define SHADER_UTILS_h

#include <glad/gl.h>
#include <string>

class ShaderUtils
{
  public:
    static const unsigned int compileShader(GLenum type, const char* path);
    static unsigned int loadTexture(std::string path, std::string directory = ".");

  private:
    static const std::string readFile(const char* path);
    static const unsigned int compile(GLenum type, const char* source);
};

#endif
