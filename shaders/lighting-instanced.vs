#version 330 core

layout (location = 0) in vec3 in_position;

#ifdef NORMAL
layout (location = 1) in vec3 in_normal;
#endif

#ifdef MATERIAL
layout (location = 2) in vec2 in_tex_coords;
#endif

layout (location = 5) in mat4 in_i_model;
layout (location = 9) in mat3 in_i_trans_inver_model; // mat3(transpose(inverse(in_i_model)))

layout(packed) uniform ub_proj_x_view
{
  mat4 proj_x_view;
};

out vsout
{
#ifdef NORMAL
  vec3 normal;
#endif

#ifdef FRAG_POS
  vec3 frag_pos;
#endif

#ifdef MATERIAL
  vec2 tex_coords;
#endif
#ifdef ENABLE_SHADOWS
  vec4 light_space_frag_pos[IN_NR_DIR_LIGHTS];
#endif
} vs_out;

#ifdef ENABLE_SHADOWS
uniform mat4 u_light_space[IN_NR_DIR_LIGHTS];
#endif

void main()
{
  vec4 position = in_i_model * vec4(in_position, 1.0);

  #ifdef NORMAL
  vs_out.normal = in_i_trans_inver_model * in_normal;
  #endif

  #ifdef FRAG_POS
  vs_out.frag_pos = vec3(position);
  #endif

  #ifdef MATERIAL
  vs_out.tex_coords = in_tex_coords;
  #endif

  #ifdef ENABLE_SHADOWS
  for(int i = 0; i < IN_NR_DIR_LIGHTS; i++)
  {
    vs_out.light_space_frag_pos[i] = u_light_space[i] * position;
  }
  #endif

  gl_Position = proj_x_view * position;
}
