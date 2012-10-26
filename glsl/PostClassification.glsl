#extension GL_ARB_shading_language_include : require

#include "/AaR3d/Texture3d"

// Transfer function.
uniform sampler1D lut1d;

// Post-classification.
vec4 post_classification (vec4 p)
{
  float f = texture3d (p);
  return texture (lut1d, f);
}

