#version 420 compatibility

#extension GL_ARB_shading_language_include : require

#include "/AaR3d/Rainbow"

void main ()
{
  gl_FragColor = rainbow (mc_slicing_coords.xyz);
}

