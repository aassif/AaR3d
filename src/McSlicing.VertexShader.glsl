#version 130
#extension GL_EXT_gpu_shader4 : enable

uniform float mc_slicing_z;
uniform vec2  mc_slicing_edge_depths [12];

out     vec4  mc_slicing_coords;

vec4 mc_slicing_vertex (int k)
{
  float z0 = mc_slicing_edge_depths[k][0];
  float z1 = mc_slicing_edge_depths[k][1];

  float r = (mc_slicing_z - z0) / (z1 - z0);

  switch (k)
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

void main ()
{
  int edge = int (gl_Vertex [0]);
  vec4 vertex = mc_slicing_vertex (edge);
  mc_slicing_coords = vertex;
  //gl_TexCoord[0] = gl_TextureMatrix[0]          * vertex;
  gl_Position    = gl_ModelViewProjectionMatrix * vertex;
}

