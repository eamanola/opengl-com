#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;
layout (location = 3) in vec3 in_tile_offset;
layout (location = 4) in vec4 in_tile_color;

layout(packed) uniform ub_proj_x_view
{
  mat4 proj_x_view;
};

out vsout
{
  vec3 normal;
  vec3 frag_pos;
  vec2 tex_coords;
  vec4 color;
} vs_out;

uniform mat4 u_model;
uniform mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))

void main()
{
  vec4 position = u_model * vec4(in_position + in_tile_offset, 1.0);

  vs_out.normal = u_trans_inver_model * in_normal;

  vs_out.frag_pos = vec3(position);

  vs_out.tex_coords = in_tex_coords;

  vs_out.color = in_tile_color;

  gl_Position = proj_x_view * position;
}
