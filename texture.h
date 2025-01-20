#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

enum TEXTURE_TYPE { TEXTURE_TYPE_DIFFUSE, TEXTURE_TYPE_SPECULAR, TEXTURE_TYPE_NORMALS };

struct Texture {
  unsigned int id;
  TEXTURE_TYPE type;
};

#endif
