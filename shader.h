#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;
    void free() const;
    void setBool(const std::string &name, const bool &value) const;
    void setInt(const std::string &name, const int &value) const;
    void setFloat(const std::string &name, const float &value) const;
    void setMat4fv(const std::string &name, const glm::mat4 &value) const;
    void setVec3fv(const std::string &name, const glm::vec3 &value) const;
    void setVec4fv(const std::string &name, const glm::vec3 &value) const;

private:
    unsigned int ID;
};
#endif

