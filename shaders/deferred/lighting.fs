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

#ifdef CALC_ATTENUATION
struct Attenuation {
  float constant;
  float linear;
  float quadratic;
};
#endif

#ifdef CALC_DIR_LIGHT // or CALC_POINT_LIGHT or CALC_SPOT_LIGHT
struct Light {
  PhongColor color;

  bool off;
};
#endif

#ifdef CALC_DIR_LIGHT
struct DirLight {
  vec3 direction;

  Light light;
};
#endif

#ifdef CALC_POINT_LIGHT
struct PointLight {
  vec3 position;
  Attenuation attenuation;

  Light light;
};
#endif

#ifdef CALC_SPOT_LIGHT
struct SpotLight {
  vec3 direction;
  float cutOff;
  float outerCutOff;

  vec3 position;
  Attenuation attenuation;

  Light light;
};
#endif

#ifdef CALC_DIR_LIGHT
mat3x4 calcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
#endif

#ifdef CALC_POINT_LIGHT
mat3x4 calcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 fragPos);
#endif

#ifdef CALC_SPOT_LIGHT
mat3x4 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 fragPos);
#endif

#ifdef CALC_ATTENUATION
float calcAttenuation(Attenuation attenuation, float dist);
#endif

#ifdef ENABLE_DIR_SHADOWS
float calcShadow(vec4 light_space_frag_pos, sampler2D shadowMap);
#endif

#ifdef ENABLE_CUBE_SHADOWS
float calcShadow_cube(vec3 frag_pos, vec3 lightPos, samplerCube shadowMap, vec3 normal);
#endif

#ifdef MATERIAL
PhongColor calcMaterialColor(Material material, vec2 texCoords);
#endif

PhongColor calcLight(vec3 fragPos, vec3 normal);

#ifdef HEIGHT_MAP
vec2 parallax(vec2 texCoords, vec3 tan_view_dir);
#endif
// end typedefs

out vec4 f_color;

// layout(std140) uniform ub_lights
layout(packed) uniform ub_lights
{
#ifdef HAS_DIR_LIGHTS
  DirLight u_dir_lights[IN_NR_DIR_LIGHTS];
#endif

#ifdef HAS_POINT_LIGHTS
  PointLight u_point_lights[IN_NR_POINT_LIGHTS];
#endif

#ifdef HAS_SPOT_LIGHTS
  SpotLight u_spot_lights[IN_NR_SPOT_LIGHTS];
#endif
};

#ifdef ENABLE_DIR_SHADOWS
uniform sampler2D u_dir_shadow_maps[IN_NR_DIR_LIGHTS];
uniform mat4 u_light_space[IN_NR_DIR_LIGHTS];
#endif
#ifdef ENABLE_CUBE_SHADOWS
uniform samplerCube u_point_shadow_maps[IN_NR_POINT_LIGHTS];
uniform float u_far;
#endif

in vec2 texCoords;
uniform sampler2D in_position;
uniform sampler2D in_normal;
uniform sampler2D in_color;
uniform sampler2D in_alpha;
uniform sampler2D in_specular;
uniform sampler2D in_emissive;
uniform sampler2D in_depth;
uniform vec3 u_view_dir;
uniform float u_shininess;

void main()
{

  float alpha = texture(in_alpha, texCoords).r;

  if (alpha < 0.1) {
    discard;
    return;
  }

  // calc normal
  vec3 fragPos = texture(in_position, texCoords).rgb;
  vec3 normal = texture(in_normal, texCoords).rgb;
  vec4 diffuseColor = vec4(texture(in_color, texCoords).rgb, 1.0);
  float specularColor = texture(in_specular, texCoords).r;
  vec3 emissive = texture(in_emissive, texCoords).rgb;
  float depth = texture(in_depth, texCoords).r;

  // calc light
  PhongColor lightColor = calcLight(fragPos, normal);

  // combine
  vec4 ambient = lightColor.ambient * diffuseColor;
  vec4 diffuse = lightColor.diffuse * diffuseColor;
  vec4 specular = lightColor.specular * specularColor;

  vec3 color = vec3(ambient + diffuse + specular) + emissive;

  f_color = vec4(color, alpha);
  gl_FragDepth = depth;

  // f_color = vec4(vec3(depth), 1.0);
}

PhongColor calcLight(vec3 fragPos, vec3 normal)
{
  mat3x4 lightColor = mat3x4(0);

#ifdef HAS_DIR_LIGHTS
  for (int i = 0; i < IN_NR_DIR_LIGHTS; i++) {
    if (!u_dir_lights[i].light.off) {
      mat3x4 lc = calcDirLight(u_dir_lights[i], normal, u_view_dir);

#ifdef ENABLE_DIR_SHADOWS
      vec4 light_space_frag_pos = u_light_space[i] * vec4(fragPos, 1.0);
      float shadow = calcShadow(light_space_frag_pos, u_dir_shadow_maps[i]);
      lc[1] *= (1 - shadow);
      lc[2] *= (1 - shadow);
#endif

      lightColor += lc;
    }
  }
#endif

#ifdef HAS_POINT_LIGHTS
  for (int i = 0; i < IN_NR_POINT_LIGHTS; i++) {
    if (!u_point_lights[i].light.off) {
      mat3x4 lc = calcPointLight(u_point_lights[i], normal, u_view_dir, fragPos);

#ifdef ENABLE_CUBE_SHADOWS
      float shadow =
        calcShadow_cube(fragPos, u_point_lights[i].position, u_point_shadow_maps[i], normal);
      lc[1] *= (1 - shadow);
      lc[2] *= (1 - shadow);
#endif

      lightColor += lc;
    }
  }
#endif

#ifdef HAS_SPOT_LIGHTS
  for (int i = 0; i < IN_NR_SPOT_LIGHTS; i++) {
    if (!u_spot_lights[i].light.off) {
      lightColor += calcSpotLight(u_spot_lights[i], normal, u_view_dir, fragPos);
    }
  }
#endif

  return PhongColor(lightColor[0], lightColor[1], lightColor[2]);
}

#ifdef CALC_ATTENUATION
float calcAttenuation(Attenuation atte, float dist)
{
  return 1.0 / (atte.constant + atte.linear * dist + atte.quadratic * (dist * dist));
}
#endif

#ifdef CALC_DIR_LIGHT
float blinnPhong(vec3 lightDir, vec3 normal, vec3 viewDir)
{
  vec3 halfway = normalize(lightDir + viewDir);
  return pow(max(dot(normal, halfway), 0.0), u_shininess);
}

mat3x4 calcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-dirLight.direction);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  // vec3 reflectDir = reflect(-lightDir, normal);
  // float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
  float spec = blinnPhong(lightDir, normal, viewDir);

  Light light = dirLight.light;
  vec4 ambient = light.color.ambient;
  vec4 diffuse = light.color.diffuse * diff;
  vec4 specular = light.color.specular * spec;

  return mat3x4(ambient, diffuse, specular);
}
#endif

#ifdef CALC_POINT_LIGHT
mat3x4 calcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 fragPos)
{
  vec3 diff = pointLight.position - fragPos;
  vec3 lightDir = normalize(diff);
  float lightDist = length(diff);

  float attenuation = calcAttenuation(pointLight.attenuation, lightDist);

  if (attenuation < 0.005)
    return mat3x4(0);

  return calcDirLight(DirLight(-lightDir, pointLight.light), normal, viewDir) * attenuation;
}
#endif

#ifdef CALC_SPOT_LIGHT
mat3x4 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 fragPos)
{
  vec3 diff = spotLight.position - fragPos;
  vec3 lightDir = normalize(diff);

  float theta = dot(lightDir, normalize(-spotLight.direction));
  float epsilon = (spotLight.cutOff - spotLight.outerCutOff);
  float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

  if (intensity < 0.05)
    return mat3x4(0);

  return calcPointLight(
           PointLight(spotLight.position, spotLight.attenuation, spotLight.light),
           normal,
           viewDir,
           fragPos
         ) *
         intensity;
}
#endif

#ifdef ENABLE_DIR_SHADOWS
float simplePcf(vec3 projCoords, sampler2D shadowMap)
{
  float shadow = 0.0;

  vec2 texSize = 1.0 / textureSize(shadowMap, 0);
  float current = projCoords.z;
  const float bias = 0.005;

  for (int x = -1; x < 1; x++) {
    for (int y = -1; y < 1; y++) {
      float depth = texture(shadowMap, projCoords.xy + vec2(x, y) * texSize).r;
      shadow += (current - bias > depth) ? 1.0 : 0.0;
    }
  }

  return shadow / 9.0;
}

float calcShadow(vec4 light_space_frag_pos, sampler2D shadowMap)
{
  // NDC
  vec3 projCoords = (light_space_frag_pos.xyz / light_space_frag_pos.w) * 0.5 + 0.5;

  // current surface from light
  float current = projCoords.z;

  // outside shadow map
  if (current > 1.0) {
    return 0.0;
  }

  return simplePcf(projCoords, shadowMap);

  // // closest surface to light
  // float closest = texture(shadowMap, projCoords.xy).r;

  // // fix shadow acne
  // const float bias = 0.005; // max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

  // float shadow = current - bias > closest ? 1.0 : 0.0;

  // return shadow;
}
#endif

#ifdef ENABLE_CUBE_SHADOWS
float calcShadow_cube(vec3 fragPos, vec3 lightPos, samplerCube shadowMap, vec3 normal)
{
  vec3 diff = fragPos - lightPos;

  float current = length(diff);

  if (current > u_far) {
    return 0.0;
  }

  float closest = texture(shadowMap, normalize(diff)).r;
  closest *= u_far;

  // float bias = 0.005;
  vec3 lightDir = normalize(-diff);
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

  float shadow = current - bias > closest ? 1.0 : 0.0;

  return shadow;
}
#endif
