#extension GL_ARB_shading_language_include : require

#include "/Aa/R3d/PostClassification"

// Material shininess.
uniform float aa_r3d_phong_shininess; // = 8.0;

// Kernel size.
uniform vec3 aa_r3d_phong_delta; // = vec3 (0.01, 0.01, 0.01);

// Gradient.
float aa_r3d_gradient_aux (vec3 p)
{
  return aa_r3d_post_classification (vec4 (p, 1)) [3];
}

vec3 aa_r3d_gradient (vec3 p)
{
  vec3 dx = vec3 (aa_r3d_phong_delta.x, 0, 0); float x0 = aa_r3d_gradient_aux (p - dx); float x1 = aa_r3d_gradient_aux (p + dx);
  vec3 dy = vec3 (0, aa_r3d_phong_delta.y, 0); float y0 = aa_r3d_gradient_aux (p - dy); float y1 = aa_r3d_gradient_aux (p + dy);
  vec3 dz = vec3 (0, 0, aa_r3d_phong_delta.z); float z0 = aa_r3d_gradient_aux (p - dz); float z1 = aa_r3d_gradient_aux (p + dz);
  return vec3 (x1 - x0, y1 - y0, z1 - z0) / sqrt (3.0);
}

vec3 boost (vec3 v, float k)
{
  return 1.0 - (1.0 - k) * (1.0 - v);
}

vec4 aa_r3d_phong (vec4 p)
{
  vec3 g = aa_r3d_gradient (p.xyz);
  float l = length (g);
  vec3 G = -normalize (g);
  vec3 N = (l != 0.0) ? normalize ((aa_gl_modelview * vec4 (G, 0.0)).xyz) : vec3 (0, 0, 1);
  vec3 L = normalize (gl_LightSource[0].position.xyz);
  vec3 H = normalize (gl_LightSource[0].halfVector.xyz);

  vec4 K        = aa_r3d_post_classification (p);
  vec3 diffuse  = K.rgb * max (dot (L, N), 0.0);
  vec3 specular = boost (K.rgb, 0.25) * l * pow (max (dot (H, N), 0.0), aa_r3d_phong_shininess);

  return vec4 (gl_LightSource[0].diffuse.rgb  * diffuse
             + gl_LightSource[0].specular.rgb * specular, K.a);
}

