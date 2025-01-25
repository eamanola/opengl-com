#version 330 core

layout(location = 0) in vec3 in_position;

layout(packed) uniform ub_proj_x_view { mat4 proj_x_view; };

uniform mat4 u_model;

void main() { gl_Position = proj_x_view * u_model * vec4(in_position, 1.0f); }
