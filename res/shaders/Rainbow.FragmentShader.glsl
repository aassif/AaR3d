#version 130

// Texture.
uniform sampler3D image_tex3d;
uniform mat4      image_scale;
uniform float     image_min;
uniform float     image_max;

// Transfer function.
uniform sampler1D lut1d;

// Kernel size.
uniform vec3 delta; // = vec3 (0.01, 0.01, 0.01);

in vec4 mc_slicing_coords;

// Window.
float window (float x)
{
  return (x - image_min) / (image_max - image_min);
}

// Post-classification.
vec4 post_classification (vec3 p0)
{
  // Texture lookup.
  vec4 v = image_scale * vec4 (p0, 1);
  float f = texture (image_tex3d, v.xyz) [0];

  // Lut lookup.
  return texture (lut1d, window (f));
}

// Gradient.
float gradient_aux (vec3 p)
{
  return post_classification (p) [3];
}

vec3 gradient (vec3 p)
{
  vec3 dx = vec3 (delta.x, 0, 0); float x0 = gradient_aux (p - dx); float x1 = gradient_aux (p + dx);
  vec3 dy = vec3 (0, delta.y, 0); float y0 = gradient_aux (p - dy); float y1 = gradient_aux (p + dy);
  vec3 dz = vec3 (0, 0, delta.z); float z0 = gradient_aux (p - dz); float z1 = gradient_aux (p + dz);
  return vec3 (x1 - x0, y1 - y0, z1 - z0) / sqrt (3.0);
}

vec4 colorize (vec3 v)
{
  return vec4 ((0.5 * v) + 0.5, 1.0);
}

vec4 rainbow (vec3 p)
{
  vec3 G = gradient (p);
  float alpha = gradient_aux (p);
  return alpha * colorize (G);
}

void main ()
{
  gl_FragColor = rainbow (mc_slicing_coords.xyz);
}

