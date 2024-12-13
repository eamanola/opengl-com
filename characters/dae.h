#ifndef DAE_H
#define DAE_H

#include "character.h"

class Dae : public Character
{
public:
  Dae();
  ~Dae();

  void draw(Shader &shader) override;
  void free() override;

private:
  unsigned int textureId;
};

#endif
