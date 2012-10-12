#extension GL_ARB_shading_language_include : require

#include "/AaR3d/PostClassification"

// Kernel size.
uniform vec3 delta; // = vec3 (0.01, 0.01, 0.01);

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

vec4 rainbow (vec4 p)
{
  vec3 G = gradient (p.xyz);
  float alpha = gradient_aux (p.xyz);
  return alpha * colorize (G);
}

