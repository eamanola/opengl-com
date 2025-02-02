#version 330 core

out vec3 v_normal;
out vec3 v_frag_pos;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

layout(packed) uniform ub_proj_x_view { mat4 proj_x_view; };

uniform mat4 u_model;
uniform mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))

void main()
{
  vec4 position = u_model * vec4(in_position, 1.0);

  v_normal = normalize(u_trans_inver_model * in_normal);
  v_frag_pos = vec3(position);

  gl_Position = proj_x_view * position;
}
