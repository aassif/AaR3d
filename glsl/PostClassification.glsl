#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/Texture"

// Transfer function.
uniform sampler1D aa_r3d_lut1d;

// Post-classification.
vec4 aa_r3d_post_classification (vec4 p)
{
  float f = aa_r3d_texture (p);
  return texture (aa_r3d_lut1d, f);
}

