#version 330 core

out vec4 f_color;

in vec3 v_frag_pos;
in vec3 v_normal; // normalized

uniform vec3 u_view_pos;
uniform samplerCube skybox;
uniform float u_ratio;

void main()
{
  vec3 i = normalize(v_frag_pos - u_view_pos);
  vec3 r;
  if (u_ratio == 0.0)
    r = reflect(i, v_normal);
  else
    r = refract(i, v_normal, u_ratio);

  f_color = vec4(texture(skybox, r).rgb, 1.0);
}
