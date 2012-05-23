#version 130

// Texture.
uniform sampler3D image3d;

in vec4 mc_slicing_coords;

// Post-classification.
vec4 texture3d ()
{
  // Texture lookup.
  vec4 t0 = gl_TextureMatrix[0] * mc_slicing_coords;
  return texture (image3d, t0.xyz);
}

void main ()
{
  gl_FragColor = texture3d ();
}

