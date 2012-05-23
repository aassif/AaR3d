#version 130

// Texture.
uniform sampler3D image3d;

// Transfer function.
uniform sampler2D lut2d;

uniform float mc_slicing_step;

in vec4 mc_slicing_coords;

// Pre-integration.
vec4 pre_integration ()
{
  // Offset.
  vec3 ray = (gl_ModelViewMatrix * mc_slicing_coords).xyz;

  // Texture lookup.
  vec4 p0 = gl_ModelViewMatrixInverse * vec4 (ray * ((ray.z - 0.5 * mc_slicing_step) / ray.z), 1);
  vec4 t0 = gl_TextureMatrix[0] * p0;
  float v0 = texture (image3d, t0.xyz) [0];

  // Texture lookup.
  vec4 p1 = gl_ModelViewMatrixInverse * vec4 (ray * ((ray.z + 0.5 * mc_slicing_step) / ray.z), 1);
  vec4 t1 = gl_TextureMatrix[0] * p1;
  float v1 = texture (image3d, t1.xyz) [0];

  // Lut lookup.
  return texture (lut2d, vec2 (v0, v1));
}

void main ()
{
  gl_FragColor = pre_integration ();
}

