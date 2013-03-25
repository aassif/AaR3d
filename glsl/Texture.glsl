#ifndef AA_R3D_TEXTURE
#define AA_R3D_TEXTURE

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"

uniform sampler3D aa_r3d_tex3d;
uniform float     aa_r3d_min;
uniform float     aa_r3d_max;

// Normalization.
float aa_r3d_normalize (float x)
{
  return clamp ((x - aa_r3d_min) / (aa_r3d_max - aa_r3d_min), 0, 1);
}

// Texture size.
ivec3 aa_r3d_size ()
{
  return textureSize (aa_r3d_tex3d, 0);
}

// Texture delta.
vec3 aa_r3d_delta ()
{
  ivec3 size = aa_r3d_size ();
  return vec3 (1.0 / size[0], 1.0 / size[1], 1.0 / size[2]);
}

// Texture lookup + normalization.
float aa_r3d_texture (vec4 p)
{
  float f = texture (aa_r3d_tex3d, p.xyz) [0];
  return aa_r3d_normalize (f);
}

#endif // AA_R3D_TEXTURE

