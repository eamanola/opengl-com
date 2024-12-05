#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

enum TEXTURE_TYPE
{
  TEXTURE_TYPE_DIFFUSE,
  TEXTURE_TYPE_SPECULAR
};

struct Texture
{
  unsigned int id;
  TEXTURE_TYPE type;
  std::string path;
};

#endif
