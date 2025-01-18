#version 330 core

#ifdef MATERIAL
struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  vec4      diffuse_color;
  vec4      specular_color;
  float     shininess;
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
struct PhongColor {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
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
float calcAttenuation(Attenuation attenuation, float distance);
#endif

#ifdef ENABLE_SHADOWS
float calcShadow(vec4 light_space_frag_pos, sampler2D shadowMap);
#endif
#ifdef ENABLE_CUBE_SHADOWS
float calcShadow_cube(vec3 frag_pos, vec3 lightPos, samplerCube shadowMap, vec3 normal);
#endif

#ifdef MATERIAL
PhongColor calcMaterialColor(Material material);
#endif

in vsout
{
#ifdef NORMAL
  vec3 normal;
#endif

#ifdef FRAG_POS
  vec3 frag_pos;
#endif

#ifdef MATERIAL
  vec2 tex_coords;
#endif

#ifdef IN_V_COLOR
  vec4 color;
#endif

#ifdef ENABLE_SHADOWS
  vec4 light_space_frag_pos[IN_NR_DIR_LIGHTS];
#endif
} fs_in;

out vec4 f_color;

#ifdef VIEW_DIR
uniform vec3 u_view_pos;
#endif

#ifdef MATERIAL
uniform Material u_material;
#endif

#ifdef ENABLE_SHADOWS
uniform sampler2D u_dir_shadow_maps[IN_NR_DIR_LIGHTS];
#endif
#ifdef ENABLE_CUBE_SHADOWS
uniform samplerCube u_point_shadow_maps[IN_NR_POINT_LIGHTS];
uniform float u_far;
#endif

layout(std140) uniform ub_lights
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


void main()
{
#ifdef NORMAL
  vec3 normal = normalize(fs_in.normal);
#endif

#ifdef VIEW_DIR
  vec3 viewDir = normalize(u_view_pos - fs_in.frag_pos);
#endif

  // mat3x4 lightColor = mat3x4(vec4(vec3(0.0), 1.0), vec4(0.0), vec4(0.0));
  mat3x4 lightColor = mat3x4(0);

#ifdef HAS_DIR_LIGHTS
  for(int i = 0; i < IN_NR_DIR_LIGHTS; i++)
  {
    if(!u_dir_lights[i].light.off)
    {
      mat3x4 lc = calcDirLight(u_dir_lights[i], normal, viewDir);

#ifdef ENABLE_SHADOWS
      float shadow = calcShadow(fs_in.light_space_frag_pos[i], u_dir_shadow_maps[i]);
      lc[1] *= (1 - shadow);
      lc[2] *= (1 - shadow);
#endif

      lightColor += lc;
    }
  }
#endif

#ifdef HAS_POINT_LIGHTS
  for(int i = 0; i < IN_NR_POINT_LIGHTS; i++)
  {
    if(!u_point_lights[i].light.off)
    {
      mat3x4 lc = calcPointLight(u_point_lights[i], normal, viewDir, fs_in.frag_pos);

#ifdef ENABLE_CUBE_SHADOWS
      float shadow = calcShadow_cube(fs_in.frag_pos, u_point_lights[i].position, u_point_shadow_maps[i], normal);
      lc[1] *= (1 - shadow);
      lc[2] *= (1 - shadow);
#endif

      lightColor += lc;
    }
  }
#endif

#ifdef HAS_SPOT_LIGHTS
  for(int i = 0; i < IN_NR_SPOT_LIGHTS; i++)
  {
    if(!u_spot_lights[i].light.off)
    {
      lightColor += calcSpotLight(u_spot_lights[i], normal, viewDir, fs_in.frag_pos);
    }
  }
#endif

// #ifdef ENABLE_CUBE_SHADOWS
//   for(int i = 0; i < IN_NR_POINT_LIGHTS; i++)
//   {
//     float shadow_inv_cube = 1 - calcShadow_cube(fs_in.frag_pos, u_point_lights[i].position, u_point_shadow_maps[i], normal);
//     lightColor[1] *= shadow_inv_cube;
//     lightColor[2] *= shadow_inv_cube;
//   }
// #endif

vec4 diffuseColor = vec4(0.0);
vec4 specularColor = vec4(0.0);

#ifdef MATERIAL
  PhongColor materialColor = calcMaterialColor(u_material);
  diffuseColor += materialColor.diffuse;
  specularColor += materialColor.specular;
#endif

#ifdef IN_V_COLOR
  diffuseColor += fs_in.color;
  specularColor += fs_in.color;
#endif

  vec4 ambient = lightColor[0] * diffuseColor;
  vec4 diffuse = lightColor[1] * diffuseColor;
  vec4 specular = lightColor[2] * specularColor;

  vec4 color = vec4(vec3(ambient + diffuse + specular), diffuseColor.a);

  if(color.a < 0.1)
  {
    discard;
    return;
  }

  f_color = color;
}

#ifdef MATERIAL
PhongColor calcMaterialColor(Material material)
{
  vec4 diffuseColor = vec4(texture(u_material.texture_diffuse1, fs_in.tex_coords));
  vec4 specularColor = vec4(texture(u_material.texture_specular1, fs_in.tex_coords));

  diffuseColor += u_material.diffuse_color;
  specularColor += u_material.specular_color;

  return PhongColor(vec4(0), diffuseColor, specularColor);
}
#endif

#ifdef CALC_ATTENUATION
float calcAttenuation(Attenuation attenuation, float distance)
{
  return 1.0 / (
    attenuation.constant
    + attenuation.linear * distance
    + attenuation.quadratic * (distance * distance)
  );
}
#endif

#ifdef CALC_DIR_LIGHT
float blinnPhong(vec3 lightDir, vec3 normal, vec3 viewDir)
{
  vec3 halfway = normalize(lightDir + viewDir);
  return pow(max(dot(normal, halfway), 0.0), u_material.shininess);
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
  vec3 lightDiff = pointLight.position - fragPos;
  vec3 lightDir = normalize(lightDiff);
  float lightDist = length(lightDiff);

  float attenuation = calcAttenuation(pointLight.attenuation, lightDist);

  return calcDirLight(
    DirLight(-lightDir, pointLight.light),
    normal,
    viewDir
  ) * attenuation;
}
#endif

#ifdef CALC_SPOT_LIGHT
mat3x4 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 fragPos)
{
  vec3 lightDiff = spotLight.position - fragPos;
  vec3 lightDir = normalize(lightDiff);

  float theta = dot(lightDir, normalize(-spotLight.direction));
  float epsilon = (spotLight.cutOff - spotLight.outerCutOff);
  float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

  return calcPointLight(
    PointLight(spotLight.position, spotLight.attenuation, spotLight.light),
    normal,
    viewDir,
    fragPos
  ) * intensity;
}
#endif

#ifdef ENABLE_SHADOWS
  float simplePcf(vec3 projCoords, sampler2D shadowMap)
  {
    float shadow = 0.0;

    vec2 texSize = 1.0 / textureSize(shadowMap, 0);
    float current = projCoords.z;
    const float bias = 0.005;

    for(int x = -1; x < 1; x++)
    {
      for(int y = -1; y < 1; y++)
      {
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
    if(current > 1.0)
    {
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
float calcShadow_cube(vec3 frag_pos, vec3 lightPos, samplerCube shadowMap, vec3 normal)
{
  vec3 diff = frag_pos - lightPos;

  float current = length(diff);

  if(current > u_far)
  {
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
