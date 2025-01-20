#version 430 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;
#ifdef NORMAL_MAP
layout (location = 3) in vec3 in_tangent;
#endif
layout (location = 5) in vec4 in_bone_ids;
layout (location = 6) in vec4 in_bone_weights;

layout(packed) uniform ub_proj_x_view
{
  mat4 proj_x_view;
};

out vsout
{
#ifdef NORMAL_MAP
  mat3 tbn;
#else
#ifdef NORMAL
  vec3 normal;
#endif
#endif
  vec3 frag_pos;
  vec2 tex_coords;
} vs_out;

const uint MAX_BONE_INFLUECE = 4;
const uint MAX_BONES = 100;

uniform mat4 u_model;
uniform mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))
uniform mat4 u_bone_transforms[MAX_BONES];

void main()
{
  mat4 boneTransform = mat4(0.0);
  for(uint i = 0; i < MAX_BONE_INFLUECE; i++)
    boneTransform += u_bone_transforms[uint(in_bone_ids[i])] * in_bone_weights[i];

  vec4 position = u_model * boneTransform * vec4(in_position, 1.0);

  #ifdef NORMAL_MAP

  vec3 t_tangent = normalize(u_trans_inver_model * mat3(boneTransform) * in_tangent);
  vec3 t_normal = normalize(u_trans_inver_model * mat3(boneTransform) * in_normal);
  vec3 t_tan_ortho = normalize(t_tangent - dot(t_tangent, t_normal) * t_normal);
  vec3 t_bitan = cross(t_normal, t_tan_ortho);

  vs_out.tbn = mat3(t_tangent, t_bitan, t_normal);
  #else
  #ifdef NORMAL
  vs_out.normal = u_trans_inver_model * mat3(boneTransform) * in_normal;
  #endif
  #endif

  vs_out.frag_pos = vec3(position);

  vs_out.tex_coords = in_tex_coords;

  gl_Position = proj_x_view * position;
}
