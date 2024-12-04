#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);

    void use(bool useThis = true);
    void free();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4fv(const std::string &name, float* value) const;
    void setVec3(const std::string &name, float* value) const;

private:
    const unsigned int compile(GLenum type, const char* source);
    const std::string readFile(const char* path);

};
#endif

