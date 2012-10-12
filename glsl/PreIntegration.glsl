#extension GL_ARB_shading_language_include : require

#include "/AaR3d/Texture3d"

// Transfer function.
uniform sampler2D lut2d;

// Distance between two slices.
uniform float mc_slicing_step;

// Pre-integration.
vec4 pre_integration (vec4 p)
{
  // Offset.
  vec3 ray = (gl_ModelViewMatrix * p).xyz;

  // Texture lookup.
  vec3 r0 = ray * ((ray.z - 0.5 * mc_slicing_step) / ray.z);
  vec4 p0 = gl_ModelViewMatrixInverse * vec4 (r0, 1);
  float f0 = texture3d (p0);

  // Texture lookup.
  vec3 r1 = ray * ((ray.z + 0.5 * mc_slicing_step) / ray.z);
  vec4 p1 = gl_ModelViewMatrixInverse * vec4 (r1, 1);
  float f1 = texture3d (p1);

  // Lut lookup.
  return texture (lut2d, vec2 (f0, f1));
}

