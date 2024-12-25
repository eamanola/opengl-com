#ifndef TIFA_H
#define TIFA_H

#include "character.h"

class Tifa : public Character
{
public:
  Tifa() : Character("assets/tifa/dancing/source/Bellydancing.fbx") { };
  ~Tifa() { };
};

#endif
