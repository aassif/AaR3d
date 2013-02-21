#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/Texture"

// Transfer function.
uniform sampler2D aa_r3d_lut2d;

// Distance between two slices.
uniform float aa_r3d_step;

// Pre-integration.
vec4 aa_r3d_pre_integration (vec4 p)
{
  // Offset.
  vec3 ray = (aa_gl_modelview * p).xyz;

  // Texture lookup.
  vec3 r0 = ray * ((ray.z - 0.5 * aa_r3d_step) / ray.z);
  vec4 p0 = aa_gl_modelview_inverse * vec4 (r0, 1);
  float f0 = aa_r3d_texture (p0);

  // Texture lookup.
  vec3 r1 = ray * ((ray.z + 0.5 * aa_r3d_step) / ray.z);
  vec4 p1 = aa_gl_modelview_inverse * vec4 (r1, 1);
  float f1 = aa_r3d_texture (p1);

  // Lut lookup.
  return texture (aa_r3d_lut2d, vec2 (f0, f1));
}

