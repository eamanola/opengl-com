#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_tex_coords;

out vec2 texCoords;

void main()
{
  texCoords = in_tex_coords;
  gl_Position = vec4(in_position, 1.0f);
}
