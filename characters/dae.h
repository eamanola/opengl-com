#ifndef DAE_H
#define DAE_H

#include "character.h"

class Dae : public Character
{
public:
  Dae() : Character("assets/dae-runner/model.dae") { }
  ~Dae() { }
};

#endif
