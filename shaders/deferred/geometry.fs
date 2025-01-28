#version 330 core

struct PhongColor {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

// typedefs
#ifdef MATERIAL
struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  sampler2D texture_emissive1;
#ifdef NORMAL_MAP
  sampler2D texture_normal1;
#endif
#ifdef HEIGHT_MAP
  sampler2D texture_height1;
#endif
  vec4 diffuse_color;
  vec4 specular_color;
  float shininess;
};
#endif

#ifdef MATERIAL
PhongColor calcMaterialColor(Material material, vec2 texCoords);
#endif

PhongColor calcColor(vec2 texCoords);

#ifdef HEIGHT_MAP
vec2 parallax(vec2 texCoords, vec3 tan_view_dir);
#endif
// end typedefs

// out vec4 f_color;
layout(location = 0) out vec3 f_frag_pos;
layout(location = 1) out vec3 f_normal;
layout(location = 2) out vec3 f_color;
layout(location = 3) out float f_alpha;
layout(location = 4) out float f_specular;
layout(location = 5) out vec3 f_emissive;
layout(location = 6) out float f_depth;

in vsout
{
#ifndef NORMAL_MAP
#ifdef NORMAL
  vec3 normal;
#endif
#endif

#ifdef FRAG_POS
  vec3 frag_pos;
#endif

#ifdef VIEW_DIR
  vec3 view_dir;
#endif

#ifdef TEX_COORDS
  vec2 tex_coords;
#endif

#ifdef IN_V_COLOR
  vec4 color;
#endif

#ifdef NORMAL_MAP
  mat3 tbn;
#endif

#ifdef HEIGHT_MAP
  vec3 tan_view_dir;
#endif
}
fs_in;

#ifdef MATERIAL
uniform Material u_material;
#endif

#ifdef HEIGHT_MAP
uniform float u_height_scale;
#endif

void main()
{
  vec2 texCoords = fs_in.tex_coords;

#ifdef HEIGHT_MAP
  texCoords = parallax(texCoords, fs_in.tan_view_dir);
  if (texCoords.x < 0.0 || texCoords.x > 1.0 || texCoords.y < 0.0 || texCoords.y > 1.0) {
    discard;
    return;
  }
#endif

  // calc color
  PhongColor color = calcColor(texCoords);
  float alpha = color.diffuse.a;

  if (alpha < 0.1) {
    discard;
    return;
  }

  // calc normal
  vec3 normal;
#ifdef NORMAL_MAP
  normal = texture(u_material.texture_normal1, texCoords).rgb;
  normal = normal * 2.0 - 1.0;
  normal = normalize(fs_in.tbn * normal);
#else
#ifdef NORMAL
  normal = normalize(fs_in.normal);
#endif
#endif
  // #ifdef NORMAL_MAP
  //   f_color = vec4(normal, 1.0) * 2 - 1;
  //   return;
  // #endif

  // combine

  vec3 emissive;
#ifdef MATERIAL
  emissive = texture(u_material.texture_emissive1, texCoords).rgb;
#endif

  f_frag_pos = fs_in.frag_pos;
  f_normal = normal;
  f_color = color.diffuse.rgb;
  f_alpha = alpha;
  f_specular = color.specular.r;
  f_emissive = emissive;
  f_depth = gl_FragCoord.z;
}

PhongColor calcColor(vec2 texCoords)
{
  vec4 diffuseColor = vec4(0.0);
  vec4 specularColor = vec4(0.0);

#ifdef MATERIAL
  PhongColor materialColor = calcMaterialColor(u_material, texCoords);
  diffuseColor += materialColor.diffuse;
  specularColor += materialColor.specular;
#endif

#ifdef IN_V_COLOR
  diffuseColor += fs_in.color;
  specularColor += fs_in.color;
#endif

  return PhongColor(vec4(0), diffuseColor, specularColor);
}

#ifdef HEIGHT_MAP
vec2 steepParallax(vec2 texCoords, vec3 tan_view_dir)
{
  const float minSteps = 8.0;
  const float maxSteps = 32.0;
  float numSteps = mix(maxSteps, minSteps, max(dot(vec3(0.0, 0.0, 1.0), tan_view_dir), 0.0));

  if (numSteps < minSteps)
    // discard;
    return vec2(-1.0);

  float stepSize = 1.0 / numSteps;

  vec2 offsetLimit = tan_view_dir.xy / tan_view_dir.z;
  vec2 p = offsetLimit * u_height_scale;
  vec2 deltaTex = p / numSteps;

  vec2 tex = texCoords;
  float height = texture(u_material.texture_height1, tex).r;
  float current = 0.0;

  while (current < height) {
    tex -= deltaTex;
    height = texture(u_material.texture_height1, tex).r;
    current += stepSize;
  }

  // Parallax Occlusion
  vec2 prevTex = tex + deltaTex;

  float after = height - current;
  float before = texture(u_material.texture_height1, prevTex).r - current + stepSize;

  float weight = after / (after - before);
  vec2 occulated = prevTex * weight + tex * (1 - weight);

  return occulated;
}

vec2 parallax(vec2 texCoords, vec3 tan_view_dir)
{
  return steepParallax(texCoords, tan_view_dir);
  // // adjust height depending on view angle, 0 perpendicular, 1 parallel
  // vec2 offsetLimit = tan_view_dir.xy / tan_view_dir.z;

  // // extra control
  // const float height_scale = 0.1;

  // float height = texture(u_material.texture_height1, texCoords).r;

  // vec2 p = offsetLimit * height * height_scale;

  // return texCoords - p;
}
#endif

#ifdef MATERIAL
PhongColor calcMaterialColor(Material material, vec2 texCoords)
{
  vec4 diffuseColor = texture(material.texture_diffuse1, texCoords);
  vec4 specularColor = vec4(vec3(texture(material.texture_specular1, texCoords).r), 0.0);

  diffuseColor += material.diffuse_color;
  specularColor += material.specular_color;

  return PhongColor(vec4(0), diffuseColor, specularColor);
}
#endif
