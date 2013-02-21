#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

uniform isampler2D aa_r3d_slicer_table;
uniform float      aa_r3d_slicer_vertex_depths [8];
uniform vec2       aa_r3d_slicer_edge_depths [12];

out     vec4       aa_r3d_position;

int aa_r3d_slicer_key (float z)
{
  int k = 0;
  for (int i = 0; i < 8; ++i)
    if (aa_r3d_slicer_vertex_depths [i] < z) k |= (1 << i);
  return k;
}

int aa_r3d_slicer_edge (int y, int x)
{
  return texelFetch (aa_r3d_slicer_table, ivec2 (x, y), 0) [0];
}

vec4 aa_r3d_slicer_vertex_aux (float z, int e)
{
  float z0 = aa_r3d_slicer_edge_depths[e][0];
  float z1 = aa_r3d_slicer_edge_depths[e][1];

  float r = (z - z0) / (z1 - z0);

  switch (e)
  {
    default:
    case  0: return vec4 (r, 0, 0, 1);
    case  1: return vec4 (r, 0, 1, 1);
    case  2: return vec4 (r, 1, 0, 1);
    case  3: return vec4 (r, 1, 1, 1);
    case  4: return vec4 (0, r, 0, 1);
    case  5: return vec4 (0, r, 1, 1);
    case  6: return vec4 (1, r, 0, 1);
    case  7: return vec4 (1, r, 1, 1);
    case  8: return vec4 (0, 0, r, 1);
    case  9: return vec4 (0, 1, r, 1);
    case 10: return vec4 (1, 0, r, 1);
    case 11: return vec4 (1, 1, r, 1);
  }
}

bool aa_r3d_slicer_vertex (float z, int k, int j)
{
  int e = aa_r3d_slicer_edge (k, j);
  if (e == -1) return false;

  aa_r3d_position = aa_r3d_slicer_vertex_aux (z, e);
  gl_Position     = aa_gl_projection * aa_gl_modelview * aa_r3d_position;
  EmitVertex ();

  return true;
}

void aa_r3d_slicer (float z)
{
  // Compute key.
  int k = aa_r3d_slicer_key (z);

  // And maybe more...
  for (int j = 0; j < 6; ++j)
    if (! aa_r3d_slicer_vertex (z, k, j)) break;

  EndPrimitive ();
}

void main ()
{
  for (int i = 0; i < gl_in.length (); ++i)
    aa_r3d_slicer (gl_in[i].gl_Position[0]);
}

