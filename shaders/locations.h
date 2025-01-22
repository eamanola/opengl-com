#ifndef LOCATIONS_H
#define LOCATIONS_H

namespace LOCATIONS
{

namespace ATTRIBS
{

const unsigned int POSITION = 0;
const unsigned int NORMAL = 1;
const unsigned int TEX_COORDS = 2;
const unsigned int TANGENTS = 3;
const unsigned int COLOR = 4;

// INSTANCED
const unsigned int INSTANCED_MODELS = 5;                            // 4 spaces mat4 5-8
const unsigned int INSTANCED_TRA_INV_MODELS = INSTANCED_MODELS + 4; // 3 spaces mat3 9-11

// SKELETAL
const unsigned int BONE_IDS = 12;
const unsigned int BONE_WEIGHTS = 13;

// floor.vs
const unsigned int FLOOR_OFFSETS = 14;
} // namespace ATTRIBS

namespace TEXTURES
{
unsigned int texture0 = 0;
const unsigned int DEFAULT = texture0++;
const unsigned int MATERIAL0 = texture0++; // diffuse, specular, normal, height
const unsigned int MATERIAL1 = texture0++;
const unsigned int MATERIAL2 = texture0++;
const unsigned int MATERIAL3 = texture0++;
const unsigned int DEBUG = texture0++;
const unsigned int SKYBOX = texture0++;
const unsigned int SKYBOX_REFLECTOR = texture0++;
const unsigned int SHADOWMAPS0 = texture0++; // varying number
} // namespace TEXTURES

} // namespace LOCATIONS

#endif
