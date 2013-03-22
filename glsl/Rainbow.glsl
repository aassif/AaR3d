#ifndef AA_R3D_RAINBOW
#define AA_R3D_RAINBOW

#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/Gradient"
#include "/Aa/R3d/PostClassification"

vec4 aa_r3d_colorize (vec3 v)
{
  return vec4 ((0.5 * v) + 0.5, 1.0);
}

vec4 aa_r3d_rainbow (vec4 p)
{
  vec3 g = aa_r3d_gradient (p.xyz);
  float l = length (g);
  float alpha = aa_r3d_post_classification (p) [3];
  return alpha * aa_r3d_colorize ((l != 0) ? (g / l) : vec3 (0));
}

#endif // AA_R3D_RAINBOW

