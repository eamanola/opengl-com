#version 330 core
out vec4 f_color;

in vec2 texCoords;

uniform sampler2D tex;

void main()
{
  f_color = vec4(texture(tex, texCoords).rgb, 1.0);
}
