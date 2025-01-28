#version 330 core

// Shadows dont need vsout, resulting in out vsout {} -> err
// use plain instead? atm compiles to samne
#ifndef NORMAL_MAP
#ifndef NORMAL
#ifndef FRAG_POS
#ifndef TEX_COORDS
#ifndef IN_V_COLOR
#ifndef ENABLE_DIR_SHADOWS
#ifndef REMOVE_VS_OUT
#define REMOVE_VS_OUT
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#ifndef REMOVE_VS_OUT
out vsout
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
vs_out;
#endif

layout(location = 0) in vec3 in_position;

#ifdef NORMAL
layout(location = 1) in vec3 in_normal;
#endif

#ifdef TEX_COORDS
layout(location = 2) in vec2 in_tex_coords;
#endif

#ifdef TANGENT
layout(location = 3) in vec3 in_tangent;
#endif

#ifdef IN_V_COLOR
layout(location = 4) in vec4 in_color;
#endif

#ifdef INSTANCED
layout(location = 5) in mat4 u_model;
layout(location = 9) in mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))
#else
uniform mat4 u_model;
uniform mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))
#endif

#ifdef SKELETAL
layout(location = 12) in vec4 in_bone_ids; // TODO: why uvec4 breaks?
layout(location = 13) in vec4 in_bone_weights;
#endif

// uniform mat4 u_proj_x_view;
layout(packed) uniform ub_proj_x_view { mat4 proj_x_view; };

#ifdef SKELETAL
const uint MAX_BONE_INFLUECE = 4u;
const uint MAX_BONES = 100u;

uniform mat4 u_bone_transforms[MAX_BONES];
#endif

#ifdef VIEW_DIR
uniform vec3 u_view_pos;
#endif

void main()
{
  mat4 model = u_model;
  mat3 trans_inver_model = u_trans_inver_model;

#ifdef SKELETAL
  mat4 boneTransform = mat4(0.0);
  for (uint i = 0u; i < MAX_BONE_INFLUECE; i++)
    boneTransform += u_bone_transforms[uint(in_bone_ids[i])] * in_bone_weights[i];

  model = model * boneTransform;
  trans_inver_model = trans_inver_model * mat3(boneTransform);
#endif

  vec4 position = model * vec4(in_position, 1.0);

#ifndef NORMAL_MAP
#ifdef NORMAL
  vs_out.normal = trans_inver_model * in_normal;
#endif
#endif

#ifdef FRAG_POS
  vs_out.frag_pos = vec3(position);
#endif

#ifdef VIEW_DIR
  vs_out.view_dir = normalize(u_view_pos - vs_out.frag_pos);
#endif

#ifdef TEX_COORDS
  vs_out.tex_coords = in_tex_coords;
#endif

#ifdef IN_V_COLOR
  vs_out.color = in_color;
#endif

#ifdef TBN
  vec3 t_tangent = normalize(trans_inver_model * in_tangent);
  vec3 t_normal = normalize(trans_inver_model * in_normal);
  vec3 t_tan_ortho = normalize(t_tangent - dot(t_tangent, t_normal) * t_normal);
  vec3 t_bitan = cross(t_normal, t_tan_ortho);

  mat3 tbn = mat3(t_tangent, t_bitan, t_normal);
#endif

#ifdef NORMAL_MAP
  vs_out.tbn = tbn;
#endif

#ifdef HEIGHT_MAP
  vs_out.tan_view_dir = normalize(transpose(tbn) * vs_out.view_dir);
#endif

  gl_Position = proj_x_view * position;
}
