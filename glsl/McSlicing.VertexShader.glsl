#version 330 core

#extension GL_EXT_gpu_shader4 : enable

uniform float aa_r3d_slicer_z;
uniform vec2  aa_r3d_slicer_edge_depths [12];

layout (location = 0) in int aa_r3d_slicer_edge;

out vec4 aa_r3d_position;

vec4 aa_r3d_slicer_vertex (int k)
{
  float z0 = aa_r3d_slicer_edge_depths[k][0];
  float z1 = aa_r3d_slicer_edge_depths[k][1];

  float r = (aa_r3d_slicer_z - z0) / (z1 - z0);

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
  aa_r3d_position = aa_r3d_slicer_vertex (aa_r3d_slicer_edge);
}

