#ifndef UMATERIAL_H
#define UMATERIAL_H

#include "color.h"
#include "shader.h"
#include "texture.h"

namespace Lighting
{

namespace u_material
{

void bindTextures(const Shader& shader, const Texture* textures, const unsigned int len);
void bindTextures(const Shader& shader, const Texture* textures);

void unbindTextures(const Shader& shader, const Texture* textures, const unsigned int len);
void unbindTextures(const Shader& shader, const Texture* textures);

void setColor(const Shader& shader, const PhongColor& color);

} // namespace u_material

} // namespace Lighting

#endif
