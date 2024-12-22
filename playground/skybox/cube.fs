#version 330 core

in vec3 v_tex_coords;

uniform samplerCube skybox;

out vec4 f_color;

void main()
{
  f_color = texture(skybox, v_tex_coords);
}
