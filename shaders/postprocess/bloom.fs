#version 330 core

out vec4 f_color;

in vec2 texCoords;

uniform sampler2D scene;
uniform sampler2D bloom;
uniform bool u_bloom_on;
uniform float u_exposure;

// vec3 reinhard(vec3 color)
// {
//   return color / (color + vec3(1.0));
// }

vec3 exposure(vec3 color) { return vec3(1.0) - exp(-color * u_exposure); }

void main()
{
  vec3 hdrColor = texture(scene, texCoords).rgb;
  if (u_bloom_on)
    hdrColor += texture(bloom, texCoords).rgb;

  // vec3 color = reinhard(hdrColor);

  vec3 color = exposure(hdrColor);

  f_color = vec4(color, 1.0);
}
