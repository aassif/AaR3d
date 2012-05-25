#version 130

// Texture.
uniform sampler3D image_tex3d;
uniform mat4      image_scale;
uniform float     image_min;
uniform float     image_max;

// Transfer function.
uniform sampler2D lut2d;

uniform float mc_slicing_step;

in vec4 mc_slicing_coords;

// Window.
float window (float x)
{
  return (x - image_min) / (image_max - image_min);
}

// Pre-integration.
vec4 pre_integration ()
{
  // Offset.
  vec3 ray = (gl_ModelViewMatrix * mc_slicing_coords).xyz;

  // Texture lookup.
  vec3 r0 = ray * ((ray.z - 0.5 * mc_slicing_step) / ray.z);
  vec4 p0 = image_scale * gl_ModelViewMatrixInverse * vec4 (r0, 1);
  float f0 = texture (image_tex3d, p0.xyz) [0];

  // Texture lookup.
  vec3 r1 = ray * ((ray.z + 0.5 * mc_slicing_step) / ray.z);
  vec4 p1 = image_scale * gl_ModelViewMatrixInverse * vec4 (r1, 1);
  float f1 = texture (image_tex3d, p1.xyz) [0];

  // Lut lookup.
  return texture (lut2d, vec2 (window (f0), window (f1)));
}

void main ()
{
  gl_FragColor = pre_integration ();
}

