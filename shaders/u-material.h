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
void bindTexture(const Shader& shader, const Texture* texture);

void unbindTextures(const Shader& shader, const Texture* textures, const unsigned int len);
void unbindTexture(const Shader& shader, const Texture* texture);

void setColor(const Shader& shader, const PhongColor& color);

} // namespace u_material

} // namespace Lighting

#endif
