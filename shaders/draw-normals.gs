#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vsout
{
  // #ifdef NORMAL
  vec3 normal;
  // #endif
}
gs_in[];

layout(packed) uniform ub_proj_x_view { mat4 proj_x_view; };

const float magnitude = 0.25;

void drawLine(int index)
{
  gl_Position = gl_in[index].gl_Position;
  EmitVertex();

  gl_Position =
    gl_in[index].gl_Position + proj_x_view * (vec4(gs_in[index].normal, 0.0) * magnitude);
  EmitVertex();

  EndPrimitive();
}

void main()
{
  drawLine(0);
  drawLine(1);
  drawLine(2);
}
