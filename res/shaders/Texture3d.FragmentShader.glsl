#version 130

// Texture.
uniform sampler3D image_tex3d;
uniform mat4      image_scale;
uniform float     image_min;
uniform float     image_max;

in vec4 mc_slicing_coords;

// Window.
vec4 window4 (vec4 v)
{
  return (v - image_min) / (image_max - image_min);
}

// Post-classification.
vec4 texture3d ()
{
  // Texture lookup.
  vec4 v = image_scale * mc_slicing_coords;
  return window4 (texture (image_tex3d, v.xyz));
}

void main ()
{
  gl_FragColor = texture3d ();
}

