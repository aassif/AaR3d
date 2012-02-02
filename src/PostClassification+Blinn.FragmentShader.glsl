#version 130

// Texture.
uniform sampler3D image3d;

// Transfer functions.
uniform int mode = 2;
uniform sampler1D lut1d;
uniform sampler2D lut2d;

// Enable Blinn-Phong.
uniform bool blinn; // = true;

// Material shininess.
uniform float shininess; // = 8.0;

// Kernel size.
uniform vec3 delta; // = vec3 (0.01, 0.01, 0.01);

uniform float mc_slicing_step;

in vec4 mc_slicing_coords;

// Post-classification.
vec4 post_classification (vec3 p0)
{
  // Texture lookup.
  vec4 t0 = gl_TextureMatrix[0] * vec4 (p0, 1);
  float v0 = texture (image3d, t0.xyz) [0];

  // Lut lookup.
  return texture (lut1d, v0);
}

// Pre-integration.
vec4 pre_integration (vec3 p, float step)
{
  // Offset.
  vec3 ray = (gl_ModelViewMatrix * vec4 (p, 1)).xyz;

  // Texture lookup.
  vec4 p0 = gl_ModelViewMatrixInverse * vec4 (ray * ((ray.z - 0.5 * step) / ray.z), 1);
  vec4 t0 = gl_TextureMatrix[0] * p0;
  float v0 = texture (image3d, t0.xyz) [0];

  // Texture lookup.
  vec4 p1 = gl_ModelViewMatrixInverse * vec4 (ray * ((ray.z + 0.5 * step) / ray.z), 1);
  vec4 t1 = gl_TextureMatrix[0] * p1;
  float v1 = texture (image3d, t1.xyz) [0];

  // Lut lookup.
  return texture (lut2d, vec2 (v0, v1));
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

vec3 colorize (vec3 v)
{
  return (0.5 * v) + 0.25;
}

vec4 rainbow (vec3 p)
{
  vec3 G = gradient (p);
  vec4 K = post_classification (p);
  return K.a * vec4 (colorize (G), 1.0);
}

vec4 lookup (vec3 p)
{
  switch (mode)
  {
    default:
    case 0: return texture (image3d, p);
    case 1: return post_classification (p);
    case 2: return pre_integration (p, mc_slicing_step);
    case 3: return rainbow (p);
  }
}

vec3 boost (vec3 v, float k)
{
  return 1.0 - (1.0 - k) * (1.0 - v);
}

vec4 post_classification_blinn (vec3 p)
{
  vec3 g = gradient (p);
  float l = length (g);
  vec3 G = -normalize (g);
  vec3 N = (l != 0.0) ? normalize ((gl_ModelViewMatrix * vec4 (G, 0.0)).xyz) : vec3 (0, 0, 1);
  vec3 L = normalize (gl_LightSource[0].position.xyz);
  vec3 H = normalize (gl_LightSource[0].halfVector.xyz);

  vec4 K        = lookup (p);
  vec3 diffuse  = K.rgb * max (dot (L, N), 0.0);
  vec3 specular = boost (K.rgb, 0.25) * l * pow (max (dot (H, N), 0.0), shininess);

  return vec4 (gl_LightSource[0].diffuse.rgb  * diffuse
             + gl_LightSource[0].specular.rgb * specular, K.a);
}

void main ()
{
  vec3 pos = mc_slicing_coords.xyz;
  gl_FragColor = blinn ? post_classification_blinn (pos) : lookup (pos);
}

