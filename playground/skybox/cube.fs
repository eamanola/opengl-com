#version 330 core

out vec4 f_color;

in vec3 v_tex_coords;

uniform samplerCube skybox;

void main() { f_color = texture(skybox, v_tex_coords); }
