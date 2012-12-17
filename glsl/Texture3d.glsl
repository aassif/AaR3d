uniform sampler3D image_tex3d;
uniform float     image_min;
uniform float     image_max;

// Window.
float window (float x)
{
  return clamp ((x - image_min) / (image_max - image_min), 0, 1);
}

// Texture lookup + window.
float texture3d (vec4 p)
{
  float f = texture (image_tex3d, p.xyz) [0];
  return window (f);
}

