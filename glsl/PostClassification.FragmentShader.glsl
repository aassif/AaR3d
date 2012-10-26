#version 420

#extension GL_ARB_shading_language_include : require

#include "/AaR3d/PostClassification"

in vec4 mc_slicing_coords;

void main ()
{
  gl_FragColor = post_classification (mc_slicing_coords);
}

