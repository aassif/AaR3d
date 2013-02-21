#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/PostClassification"

// Kernel size.
uniform vec3 aa_r3d_rainbow_delta; // = vec3 (0.01, 0.01, 0.01);

// Gradient.
float aa_r3d_gradient_aux (vec3 p)
{
  return aa_r3d_post_classification (vec4 (p, 1)) [3];
}

vec3 aa_r3d_gradient (vec3 p)
{
  vec3 dx = vec3 (aa_r3d_rainbow_delta.x, 0, 0);
  vec3 dy = vec3 (0, aa_r3d_rainbow_delta.y, 0);
  vec3 dz = vec3 (0, 0, aa_r3d_rainbow_delta.z);

  float x0 = aa_r3d_gradient_aux (p - dx); float x1 = aa_r3d_gradient_aux (p + dx);
  float y0 = aa_r3d_gradient_aux (p - dy); float y1 = aa_r3d_gradient_aux (p + dy);
  float z0 = aa_r3d_gradient_aux (p - dz); float z1 = aa_r3d_gradient_aux (p + dz);

  return 0.5 * vec3 (x1 - x0, y1 - y0, z1 - z0);
}

vec4 aa_r3d_colorize (vec3 v)
{
  return vec4 ((0.5 * v) + 0.5, 1.0);
}

vec4 aa_r3d_rainbow (vec4 p)
{
  vec3 G = aa_r3d_gradient (p.xyz);
  float alpha = aa_r3d_gradient_aux (p.xyz);
  return alpha * aa_r3d_colorize (G);
}

