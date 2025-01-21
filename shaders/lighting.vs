#version 330 core

layout (location = 0) in vec3 in_position;

#ifdef NORMAL
layout (location = 1) in vec3 in_normal;
#endif

#ifdef TEX_COORDS
layout (location = 2) in vec2 in_tex_coords;
#endif

#ifdef TANGENT
layout (location = 3) in vec3 in_tangent;
#endif

#ifdef IN_V_COLOR
layout (location = 4) in vec4 in_color;
#endif

#ifdef SKELETAL
layout (location = 5) in vec4 in_bone_ids;
layout (location = 6) in vec4 in_bone_weights;
#endif

// uniform mat4 u_proj_x_view;
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

#ifdef FRAG_POS
  vec3 frag_pos;
#endif

#ifdef TEX_COORDS
  vec2 tex_coords;
#endif

#ifdef IN_V_COLOR
  vec4 color;
#endif
} vs_out;

uniform mat4 u_model;
uniform mat3 u_trans_inver_model; // mat3(transpose(inverse(u_model)))

#ifdef SKELETAL
const uint MAX_BONE_INFLUECE = 4u;
const uint MAX_BONES = 100u;

uniform mat4 u_bone_transforms[MAX_BONES];
#endif

void main()
{
  mat4 model = u_model;
  mat3 trans_inver_model = u_trans_inver_model;

#ifdef SKELETAL
  mat4 boneTransform = mat4(0.0);
  for(uint i = 0u; i < MAX_BONE_INFLUECE; i++)
    boneTransform += u_bone_transforms[uint(in_bone_ids[i])] * in_bone_weights[i];

  model = model * boneTransform;
  trans_inver_model = trans_inver_model * mat3(boneTransform);
#endif

  vec4 position = model * vec4(in_position, 1.0);

#ifdef NORMAL_MAP
  vec3 t_tangent = normalize(trans_inver_model * in_tangent);
  vec3 t_normal = normalize(trans_inver_model * in_normal);
  vec3 t_tan_ortho = normalize(t_tangent - dot(t_tangent, t_normal) * t_normal);
  vec3 t_bitan = cross(t_normal, t_tan_ortho);

  vs_out.tbn = mat3(t_tangent, t_bitan, t_normal);
#else
  #ifdef NORMAL
  vs_out.normal = trans_inver_model * in_normal;
  #endif
#endif

#ifdef FRAG_POS
  vs_out.frag_pos = vec3(position);
#endif

#ifdef TEX_COORDS
  vs_out.tex_coords = in_tex_coords;
#endif

#ifdef IN_V_COLOR
  vs_out.color = in_color;
#endif

  gl_Position = proj_x_view * position;
}
