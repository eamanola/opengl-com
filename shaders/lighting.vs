#version 330 core

layout (location = 0) in vec3 in_position;

#ifdef NORMAL
layout (location = 1) in vec3 in_normal;
#endif

#ifdef MATERIAL
layout (location = 2) in vec2 in_tex_coords;
#endif

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
} vs_out;

// uniform mat4 u_proj_x_view;
uniform mat4 u_model;
uniform mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))

void main()
{
  #ifdef NORMAL
  vs_out.normal = u_trans_inver_model * in_normal;
  #endif

  #ifdef FRAG_POS
  vs_out.frag_pos = vec3(u_model * vec4(in_position, 1.0));
  #endif

  #ifdef MATERIAL
  vs_out.tex_coords = in_tex_coords;
  #endif

  gl_Position = proj_x_view * u_model * vec4(in_position, 1.0f);
}
