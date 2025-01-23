#version 330 core
out vec4 f_color;

in vec2 texCoords;

uniform sampler2D tex;

// vec3 reinhard(vec3 color)
// {
//   return color / (color + vec3(1.0));
// }

vec3 exposure(vec3 color)
{
  float exposure = 1.0;
  return vec3(1.0) - exp(-color * exposure);
}

void main()
{
  vec3 hdrColor = texture(tex, texCoords).rgb;

  // vec3 color = reinhard(hdrColor);

  vec3 color = exposure(hdrColor);

  f_color = vec4(color, 1.0);
}
