#ifndef ICARUS_H
#define ICARUS_H

#include "character.h"

class Icarus : public Character
{
public:
  Icarus() : Character("assets/robo/scene.gltf") { }
  ~Icarus() { }
};

#endif
