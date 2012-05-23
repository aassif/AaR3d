#version 130

// Texture.
uniform sampler3D image3d;

// Transfer function.
uniform sampler1D lut1d;

in vec4 mc_slicing_coords;

// Post-classification.
vec4 post_classification ()
{
  // Texture lookup.
  vec4 t0 = gl_TextureMatrix[0] * mc_slicing_coords;
  float v0 = texture (image3d, t0.xyz) [0];

  // Lut lookup.
  return texture (lut1d, v0);
}

void main ()
{
  gl_FragColor = post_classification ();
}

