#version 420 compatibility

#extension GL_ARB_shading_language_include : require

#include "/AaR3d/PreIntegration"

in vec4 mc_slicing_coords;

void main ()
{
  gl_FragColor = pre_integration (mc_slicing_coords);
}

