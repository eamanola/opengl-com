#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

out vec3 v_normal;
out vec3 v_frag_pos;

uniform mat4 u_model;
uniform mat4 u_proj_x_view;

void main()
{
  vec4 position = u_model * vec4(in_position, 1.0);

  v_normal = normalize(mat3(transpose(inverse(u_model))) * in_normal);
  v_frag_pos = vec3(position);

  gl_Position = u_proj_x_view * position;
}
