#version 330 core

out vec4 f_color;

in vec2 texCoords;

uniform sampler2D tex;

uniform bool horizontal;
// uniform float weights[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
const float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
  vec2 offsetSize = 1.0 / textureSize(tex, 0);
  vec3 result = texture(tex, texCoords).rgb * weights[0];

  for (int i = 1; i < 5; i++) {
    vec2 offset = horizontal ? vec2(offsetSize.x * i, 0.0) : vec2(0.0, offsetSize.y * i);
    vec3 before = texture(tex, texCoords - offset).rgb;
    vec3 after = texture(tex, texCoords + offset).rgb;
    result += (before + after) * weights[i];
  }

  f_color = vec4(result, 1.0);
}
