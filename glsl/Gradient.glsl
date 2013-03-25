#ifndef AA_R3D_GRADIENT
#define AA_R3D_GRADIENT

#include "/Aa/R3d/Texture"

// Gradient.
float aa_r3d_gradient_aux (vec3 p)
{
  return aa_r3d_texture (vec4 (p, 1));
}

vec3 aa_r3d_gradient (vec3 p)
{
  vec3 d = aa_r3d_delta ();
  vec3 dx = vec3 (d[0], 0, 0); float x0 = aa_r3d_gradient_aux (p - dx); float x1 = aa_r3d_gradient_aux (p + dx);
  vec3 dy = vec3 (0, d[1], 0); float y0 = aa_r3d_gradient_aux (p - dy); float y1 = aa_r3d_gradient_aux (p + dy);
  vec3 dz = vec3 (0, 0, d[2]); float z0 = aa_r3d_gradient_aux (p - dz); float z1 = aa_r3d_gradient_aux (p + dz);
  return 0.5 * vec3 (x0 - x1, y0 - y1, z0 - z1);
}

#endif // AA_R3D_GRADIENT

