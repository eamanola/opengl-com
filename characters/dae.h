#ifndef DAE_H
#define DAE_H

#include "character.h"

class Dae : public Character
{
public:
  Dae();
  ~Dae();

  void free() override;
};

#endif
