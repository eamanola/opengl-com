#version 330 core

out vec2 texCoords;

layout(location = 0) in vec3 in_position;
layout(location = 2) in vec2 in_tex_coords;

void main()
{
  texCoords = in_tex_coords;

  gl_Position = vec4(in_position, 1.0);
}
