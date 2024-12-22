#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;

out vec3 v_normal;
out vec3 v_frag_pos;
out vec2 v_tex_coords;

uniform mat4 u_proj_x_view;
uniform mat4 u_model;

void main()
{
  v_normal = mat3(transpose(inverse(u_model))) * in_normal;
  v_frag_pos = vec3(u_model * vec4(in_position, 1.0));
  v_tex_coords = in_tex_coords;

  gl_Position = u_proj_x_view * u_model * vec4(in_position, 1.0f);
}
