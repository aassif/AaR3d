#version 130

// Texture.
uniform sampler3D image_tex3d;
uniform mat4      image_scale;
uniform float     image_min;
uniform float     image_max;

// Transfer function.
uniform sampler1D lut1d;

// Material shininess.
uniform float shininess; // = 8.0;

// Kernel size.
uniform vec3 delta; // = vec3 (0.01, 0.01, 0.01);

in vec4 mc_slicing_coords;

// Window.
float window (float x)
{
  return (x - image_min) / (image_max - image_min);
}

// Post-classification.
vec4 post_classification (vec3 p)
{
  vec4 v = image_scale * vec4 (p, 1);
  float f = texture (image_tex3d, v.xyz) [0];
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

vec3 colorize (vec3 v)
{
  return (0.5 * v) + 0.5;
}

vec4 rainbow (vec3 p)
{
  vec3 G = gradient (p);
  float k = gradient_aux (p);
  return k * vec4 (colorize (G), 1.0);
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

  vec4 K        = post_classification (p);
  vec3 diffuse  = K.rgb * max (dot (L, N), 0.0);
  vec3 specular = boost (K.rgb, 0.25) * l * pow (max (dot (H, N), 0.0), shininess);

  return vec4 (gl_LightSource[0].diffuse.rgb  * diffuse
             + gl_LightSource[0].specular.rgb * specular, K.a);
}

void main ()
{
  gl_FragColor = post_classification_blinn (mc_slicing_coords.xyz);
}

