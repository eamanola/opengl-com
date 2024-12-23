#version 330 core

layout (location = 0) in vec3 in_position;

#ifdef NORMAL
layout (location = 1) in vec3 in_normal;
#endif

#ifdef MATERIAL
layout (location = 2) in vec2 in_tex_coords;
#endif

#ifdef NORMAL
out vec3 v_normal;
#endif

#ifdef FRAG_POS
out vec3 v_frag_pos;
#endif

#ifdef MATERIAL
out vec2 v_tex_coords;
#endif

uniform mat4 u_proj_x_view;
uniform mat4 u_model;

void main()
{
  #ifdef NORMAL
  v_normal = mat3(transpose(inverse(u_model))) * in_normal;
  #endif

  #ifdef FRAG_POS
  v_frag_pos = vec3(u_model * vec4(in_position, 1.0));
  #endif

  #ifdef MATERIAL
  v_tex_coords = in_tex_coords;
  #endif

  gl_Position = u_proj_x_view * u_model * vec4(in_position, 1.0f);
}
