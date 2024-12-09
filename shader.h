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
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);

    void use(bool useThis = true);
    void free();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4fv(const std::string &name, glm::mat4 value) const;
    void setVec3fv(const std::string &name, glm::vec3 value) const;

};
#endif

