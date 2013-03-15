#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/Phong"
#include "/Aa/R3d/Fragment"

void main ()
{
  aa_r3d_fragment_color = aa_r3d_phong (aa_r3d_position);
}

