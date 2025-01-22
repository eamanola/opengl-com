#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

enum TEXTURE_TYPE {
  TEXTURE_TYPE_DIFFUSE,
  TEXTURE_TYPE_SPECULAR,
  TEXTURE_TYPE_NORMALS,
  TEXTURE_TYPE_HEIGHT,
  TEXTURE_TYPE_EMISSIVE
};

struct Texture {
  unsigned int id;
  TEXTURE_TYPE type;
};

#endif
