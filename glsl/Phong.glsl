#ifndef AA_R3D_PHONG
#define AA_R3D_PHONG

#extension GL_ARB_shading_language_include : require

#include "/Aa/Phong"
#include "/Aa/R3d/Gradient"
#include "/Aa/R3d/PostClassification"

vec4 aa_r3d_phong (vec4 p)
{
  vec3 g = aa_r3d_gradient (p.xyz);
  float l = length (g);
  vec3 n = (l != 0.0) ? aa_gl_normal_matrix * (g / l) : vec3 (0, 0, 1);
  vec4 c = aa_r3d_post_classification (p);
  return aa_phong (n, c, c.a); // pre-multiplied white.
}

#endif // AA_R3D_PHONG

