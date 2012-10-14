#version 420 compatibility

#extension GL_ARB_shading_language_include : require

#include "/AaR3d/Rainbow"

in vec4 mc_slicing_coords;

void main ()
{
  gl_FragColor = rainbow (mc_slicing_coords);
}

