#ifndef AA_R3D_PRE_INTEGRATION
#define AA_R3D_PRE_INTEGRATION

#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/Texture"

// Transfer function.
uniform sampler2D aa_r3d_lut2d;

// Distance between two slices.
uniform float aa_r3d_step;

// Front and back samples.
mat2x4 aa_r3d_pre_integration_init (vec4 p)
{
  // Offset.
  vec3 ray = (aa_gl_modelview * p).xyz;

  // Texture lookup.
  vec3 r0 = ray * ((ray.z - 0.5 * aa_r3d_step) / ray.z);
  vec4 p0 = aa_gl_modelview_inverse * vec4 (r0, 1);

  // Texture lookup.
  vec3 r1 = ray * ((ray.z + 0.5 * aa_r3d_step) / ray.z);
  vec4 p1 = aa_gl_modelview_inverse * vec4 (r1, 1);

  return mat2x4 (p0, p1);
}

// Pre-integration.
vec4 aa_r3d_pre_integration_lookup (vec4 p0, vec4 p1)
{
  float f0 = aa_r3d_texture (p0);
  float f1 = aa_r3d_texture (p1);
  return texture (aa_r3d_lut2d, vec2 (f0, f1));
}

// Pre-integration.
vec4 aa_r3d_pre_integration (vec4 p)
{
  mat2x4 m = aa_r3d_pre_integration_init (p);
  return aa_r3d_pre_integration_lookup (m[0], m[1]);
}

#endif // AA_R3D_PRE_INTEGRATION

