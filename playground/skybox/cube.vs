#version 330 core

out vec3 v_tex_coords;

layout(location = 0) in vec3 in_position;

uniform mat4 u_proj_x_view;

void main()
{
  v_tex_coords = in_position;
  gl_Position = vec4(u_proj_x_view * vec4(in_position, 1.0)).xyzz;
}
