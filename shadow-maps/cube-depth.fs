#version 330 core
in vec4 FragPos;

uniform vec3 u_light_pos;
uniform float u_far;

void main()
{
  float distance = length(FragPos.xyz - u_light_pos);

  distance = distance / u_far;

  gl_FragDepth = distance;
}
