#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"
#include "/Aa/Phong"
#include "/Aa/R3d/Texture"
#include "/Aa/R3d/PostClassification"
#include "/Aa/R3d/Phong" // gradient
#include "/Aa/R3d/PreIntegration"
#include "/Aa/R3d/Surface"
#include "/Aa/R3d/Fragment"

void main ()
{
  aa_r3d_fragment_color = aa_r3d_surface (aa_r3d_position);
}

