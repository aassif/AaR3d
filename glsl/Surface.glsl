#ifndef AA_R3D_SURFACE
#define AA_R3D_SURFACE

#extension GL_ARB_shading_language_include : require

#include "/Aa/Phong"
#include "/Aa/R3d/Gradient"
#include "/Aa/R3d/PreIntegration"

// Pre-integration.
vec4 aa_r3d_surface (vec4 p)
{
  mat2x4 m = aa_r3d_pre_integration_init (p);

  vec4 p0 = m[0];
  vec4 p1 = m[1];

  vec4 color = aa_r3d_pre_integration_lookup (p0, p1);

  vec3 g0 = aa_r3d_gradient (p0.xyz); float l0 = length (g0);
  vec3 g1 = aa_r3d_gradient (p1.xyz); float l1 = length (g1);

  vec3 n0 = (l0 != 0) ? aa_gl_normal_matrix * (g0 / l0) : vec3 (0, 0, 1);
  vec3 n1 = (l1 != 0) ? aa_gl_normal_matrix * (g1 / l1) : vec3 (0, 0, 1);

  vec4 c0 = aa_phong (n0, color);
  vec4 c1 = aa_phong (n1, color);

  return 0.5 * (c0 + c1);
}

#endif // AA_R3D_SURFACE

