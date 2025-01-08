#ifndef UMATERIAL_H
#define UMATERIAL_H

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

} // namespace u_material

} // namespace Lighting

#endif
