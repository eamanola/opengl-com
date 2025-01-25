#version 330 core

out vsout
{
  vec3 normal;
  vec3 frag_pos;
  vec3 view_dir;
  vec2 tex_coords;
  vec4 color;
#ifdef ENABLE_DIR_SHADOWS
  vec4 light_space_frag_pos[IN_NR_DIR_LIGHTS];
#endif
}
vs_out;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coords;
layout(location = 4) in vec4 in_color;
layout(location = 14) in vec3 in_offset;

layout(packed) uniform ub_proj_x_view { mat4 proj_x_view; };

uniform mat4 u_model;
uniform mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))

#ifdef ENABLE_DIR_SHADOWS
uniform mat4 u_light_space[IN_NR_DIR_LIGHTS];
#endif

uniform vec3 u_view_pos;

void main()
{
  vec4 position = u_model * vec4(in_position + in_offset, 1.0);

  vs_out.normal = u_trans_inver_model * in_normal;

  vs_out.frag_pos = vec3(position);

  vs_out.view_dir = normalize(u_view_pos - vs_out.frag_pos);

  vs_out.tex_coords = in_tex_coords;

  vs_out.color = in_color;

#ifdef ENABLE_DIR_SHADOWS
  for (int i = 0; i < IN_NR_DIR_LIGHTS; i++) {
    vs_out.light_space_frag_pos[i] = u_light_space[i] * position;
  }
#endif

  gl_Position = proj_x_view * position;
}
