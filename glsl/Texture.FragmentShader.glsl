#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/Texture"
#include "/Aa/R3d/FragmentShader"

void main ()
{
  aa_r3d_fragment_color = vec4 (aa_r3d_texture (aa_r3d_position));
}

