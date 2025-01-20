#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vsout
{
#ifdef NORMAL
  vec3 normal;
#endif

#ifdef FRAG_POS
  vec3 frag_pos;
#endif

#ifdef TEX_COORDS
  vec2 tex_coords;
#endif
} gs_in[];

out vsout
{
#ifdef NORMAL
  vec3 normal;
#endif

#ifdef FRAG_POS
  vec3 frag_pos;
#endif

#ifdef TEX_COORDS
  vec2 tex_coords;
#endif
} gs_out;

void main() {
  gl_Position = gl_in[0].gl_Position;
  gs_out.normal = gs_in[0].normal;
  gs_out.frag_pos = gs_in[0].frag_pos;
  gs_out.tex_coords = gs_in[0].tex_coords;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  gs_out.normal = gs_in[1].normal;
  gs_out.frag_pos = gs_in[1].frag_pos;
  gs_out.tex_coords = gs_in[1].tex_coords;
  EmitVertex();

  gl_Position = gl_in[2].gl_Position;
  gs_out.normal = gs_in[2].normal;
  gs_out.frag_pos = gs_in[2].frag_pos;
  gs_out.tex_coords = gs_in[2].tex_coords;
  EmitVertex();

  EndPrimitive();
}
