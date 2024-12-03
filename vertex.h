#ifndef VERTEX_H
#define VERTEX_H

#include "position.h"
#include "rgba.h"
#include "texcoords.h"
#include "normal.h"

struct Vertex
{
  Position position;
  RGBA rgba;
  TexCoords texCoords;
  Normal normal;
};

#endif
