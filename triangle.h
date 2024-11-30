#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle
{
private:
  unsigned int program;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
public:
  Triangle(/* args */);
  ~Triangle();

  bool compile();
  void render();
  void free();
};

#endif
