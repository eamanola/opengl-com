#version 330 core

#ifdef MATERIAL
struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
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
struct Light {
  bool off;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
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

#ifdef NORMAL
in vec3 v_normal;
#endif

#ifdef FRAG_POS
in vec3 v_frag_pos;
#endif

#ifdef MATERIAL
in vec2 v_tex_coords;
#endif

out vec4 f_color;

#ifdef VIEW_DIR
uniform vec3 u_view_pos;
#endif

#ifdef MATERIAL
uniform Material u_material;
#endif

#ifdef HAS_DIR_LIGHTS
uniform DirLight u_dir_lights[IN_NR_DIR_LIGHTS];
#endif

#ifdef HAS_POINT_LIGHTS
uniform PointLight u_point_lights[IN_NR_POINT_LIGHTS];
#endif

#ifdef HAS_SPOT_LIGHTS
uniform SpotLight u_spot_lights[IN_NR_SPOT_LIGHTS];
#endif

uniform vec4 u_color;

void main()
{
  vec4 color = vec4(0.0);

#ifdef NORMAL
  vec3 normal = normalize(v_normal);
#endif

#ifdef VIEW_DIR
  vec3 viewDir = normalize(u_view_pos - v_frag_pos);
#endif

#ifdef HAS_DIR_LIGHTS
  for(int i = 0; i < IN_NR_DIR_LIGHTS; i++)
  {
    if(!u_dir_lights[i].light.off)
    {
      mat3x4 dirLightColor = calcDirLight(u_dir_lights[i], normal, viewDir);
      color += dirLightColor[0] + dirLightColor[1] + dirLightColor[2];
    }
  }
#endif

#ifdef HAS_POINT_LIGHTS
  for(int i = 0; i < IN_NR_POINT_LIGHTS; i++)
  {
    if(!u_point_lights[i].light.off)
    {
      mat3x4 pointLightColor = calcPointLight(u_point_lights[i], normal, viewDir, v_frag_pos);
      color += pointLightColor[0] + pointLightColor[1] + pointLightColor[2];
    }
  }
#endif

#ifdef HAS_SPOT_LIGHTS
  for(int i = 0; i < IN_NR_SPOT_LIGHTS; i++)
  {
    if(!u_spot_lights[i].light.off)
    {
      mat3x4 spotLightColor = calcSpotLight(u_spot_lights[i], normal, viewDir, v_frag_pos);
      color += spotLightColor[0] + spotLightColor[1] + spotLightColor[2];
    }
  }
#endif

  if(color == vec4(0.0))
  {
    color = u_color;
  }
  else if (u_color != vec4(0.0))
  {
    color += u_color;
  }

  if(color.a < 0.1)
  {
    discard;
    return;
  }

  f_color = color;
}

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
mat3x4 calcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-dirLight.direction);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

  vec4 diffuseColor = vec4(texture(u_material.texture_diffuse1, v_tex_coords));
  vec4 specularColor = vec4(texture(u_material.texture_specular1, v_tex_coords));

  Light light = dirLight.light;
  vec4 ambient = light.ambient * diffuseColor;
  vec4 diffuse = light.diffuse * diff * diffuseColor;
  vec4 specular = light.specular * spec * specularColor;

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
