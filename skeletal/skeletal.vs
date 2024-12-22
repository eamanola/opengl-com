#version 430 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;
layout (location = 3) in vec4 in_bone_ids;
layout (location = 4) in vec4 in_bone_weights;

out vec2 v_tex_coords;
out vec3 v_normal;
out vec3 v_frag_pos;

const uint MAX_BONE_INFLUECE = 4;
const uint MAX_BONES = 100;

uniform mat4 u_proj_x_view;
uniform mat4 u_model;
uniform mat4 u_bone_transforms[MAX_BONES];

void main()
{
  mat4 boneTransform = mat4(0.0);
  for(uint i = 0; i < MAX_BONE_INFLUECE; i++)
    boneTransform += u_bone_transforms[uint(in_bone_ids[i])] * in_bone_weights[i];

  vec4 position = boneTransform * vec4(in_position, 1.0);

  v_tex_coords = in_tex_coords;
  v_normal = mat3(transpose(inverse(u_model * boneTransform))) * in_normal;
  v_frag_pos = vec3(u_model * position);

  gl_Position = u_proj_x_view * u_model * position;
}