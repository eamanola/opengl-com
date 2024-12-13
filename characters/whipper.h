#ifndef WHIPPER_H
#define WHIPPER_H

#include "character.h"

class Whipper : public Character
{
public:
  Whipper();
  ~Whipper();

  void draw(Shader &shader) override;
  void free() override;

private:
  unsigned int textureId;
};

#endif
