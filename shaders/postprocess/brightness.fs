#version 330 core

out vec4 f_brightness;

in vec2 texCoords;

uniform sampler2D tex;

void main()
{
  vec3 color = texture(tex, texCoords).rgb;

  float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));

  f_brightness = vec4(brightness > 1.0 ? color : vec3(0.0), 1.0);
}
