#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/Rainbow"
#include "/Aa/R3d/FragmentShader"

void main ()
{
  aa_r3d_fragment_color = aa_r3d_rainbow (aa_r3d_position);
}

