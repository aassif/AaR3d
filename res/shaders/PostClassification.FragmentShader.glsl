#version 130

// Texture.
uniform sampler3D image_tex3d;
uniform mat4      image_scale;
uniform float     image_min;
uniform float     image_max;

// Transfer function.
uniform sampler1D lut1d;

in vec4 mc_slicing_coords;

// Window.
float window (float x)
{
  return (x - image_min) / (image_max - image_min);
}

// Post-classification.
vec4 post_classification (vec4 p)
{
  vec4 v = image_scale * p;
  float f = texture (image_tex3d, v.xyz) [0];
  return texture (lut1d, window (f));
}

void main ()
{
  gl_FragColor = post_classification (mc_slicing_coords);
}

