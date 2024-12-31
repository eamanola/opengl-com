#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

class Shader
{
public:
    Shader(
        const char* vertexPath,
        const char* fragmentPath,
        const std::vector<std::string>& prependTexts = {},
        const std::vector<std::string>& prependFiles = {}
    );

    void use() const;
    void free() const;
    void setBool(const char* name, const bool &value) const;
    void setInt(const char* name, const int &value) const;
    void setFloat(const char* name, const float &value) const;
    void setMat4fv(const char* name, const glm::mat4 &value) const;
    void setVec3fv(const char* name, const glm::vec3 &value) const;
    void setVec4fv(const char* name, const glm::vec4 &value) const;

private:
    unsigned int ID;
};
#endif

