#version 130
//#version 420 compatibility

#extension GL_ARB_shading_language_include : require

#include "/AaR3d/Blinn"

in vec4 mc_slicing_coords;

void main ()
{
  gl_FragColor = post_classification_blinn (mc_slicing_coords);
}

