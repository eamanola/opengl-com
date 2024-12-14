#ifndef SHADER_UTILS_h
#define SHADER_UTILS_h

#include <glad/gl.h>
#include <string>

class ShaderUtils
{
  public:
    static const unsigned int compileShader(GLenum type, const char* path);
    static unsigned int loadTexture(const char* path);
    static unsigned int loadTexture(const unsigned char* const buffer, unsigned int len);

  private:
    static const std::string readFile(const char* path);
    static const unsigned int compile(GLenum type, const char* source);
    static const unsigned int getTexture(int width, int height, int nrChannel, unsigned char* data);
};

#endif
