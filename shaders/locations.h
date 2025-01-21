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
const unsigned int DEFAULT = 0;
const unsigned int MATERIAL0 = 1; // diffuse, specular & normal
const unsigned int MATERIAL1 = 2;
const unsigned int MATERIAL2 = 3;
const unsigned int DEBUG = 4;
const unsigned int SKYBOX = 5;
const unsigned int SKYBOX_REFLECTOR = 6;
const unsigned int SHADOWMAPS0 = 7; // varying number
} // namespace TEXTURES

} // namespace LOCATIONS

#endif
