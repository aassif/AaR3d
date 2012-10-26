#version 130
//#version 420 compatibility

#extension GL_ARB_shading_language_include : require

#include "/AaR3d/Texture3d"

in vec4 mc_slicing_coords;

void main ()
{
  gl_FragColor = vec4 (texture3d (mc_slicing_coords));
}

