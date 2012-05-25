#version 130
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable

//layout(points) in;
//layout(triangle_strip, max_vertices=6) out;

uniform isampler2D mc_slicing_table;
uniform float      mc_slicing_vertex_depths [8];
uniform vec2       mc_slicing_edge_depths [12];

out     vec4       mc_slicing_coords;

int mc_slicing_key (float z)
{
  int k = 0;
  for (int i = 0; i < 8; ++i)
    if (mc_slicing_vertex_depths [i] < z) k |= (1 << i);
  return k;
}

int mc_slicing_edge (int y, int x)
{
  return texelFetch (mc_slicing_table, ivec2 (x, y), 0) [0];
}

vec4 mc_slicing_vertex_aux (float z, int e)
{
  float z0 = mc_slicing_edge_depths[e][0];
  float z1 = mc_slicing_edge_depths[e][1];

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

bool mc_slicing_vertex (float z, int k, int j)
{
  int e = mc_slicing_edge (k, j);
  if (e == -1) return false;

  mc_slicing_coords = mc_slicing_vertex_aux (z, e);
  gl_Position = gl_ModelViewProjectionMatrix * mc_slicing_coords;
  EmitVertex ();

  return true;
}

void mc_slicing (float z)
{
  // Compute key.
  int k = mc_slicing_key (z);

  // And maybe more...
  for (int j = 0; j < 6; ++j)
    if (! mc_slicing_vertex (z, k, j)) break;

  EndPrimitive ();
}

void main ()
{
  for (int i = 0; i < gl_VerticesIn; ++i)
    mc_slicing (gl_PositionIn[i][0]);
}

