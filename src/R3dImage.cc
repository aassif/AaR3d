#include <fstream>
#include <cmath>
#include "R3dImage.h"

using namespace std;

#ifdef DEBUG
#define ASSERT(x) assert (x)
#else
#define ASSERT(x)
#endif

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::Image //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Image::Image (const uvec3 & dims,
                  const  vec3 & scale,
                  const  vec3 & position,
                  const  mat3 & orientation) :
      m_image (uvec3 (0u)),
      m_scale (scale),
      m_position (position),
      m_orientation (orientation)
    {
      this->resize (dims);
    }

    Image::~Image ()
    {
    }

    void Image::resize (const uvec3 & dims)
    {
      m_image = Im<3, Mono8> (dims);
    }

    void Image::setScale (const vec3 & scale)
    {
      m_scale = scale;
    }

    void Image::setPosition (const vec3 & position)
    {
      m_position = position;
    }

    void Image::setOrientation (const mat3 & orientation)
    {
      m_orientation = orientation;
    }

    mat4 Image::transform () const
    {
      return Translation (m_position)
           * mat4 (m_orientation)
           * Scale (m_scale);
    }

    Mono8::Pixel * Image::begin () {return m_image.begin ();}
    Mono8::Pixel * Image::end   () {return m_image.end   ();}

    const Mono8::Pixel * Image::begin () const {return m_image.begin ();}
    const Mono8::Pixel * Image::end   () const {return m_image.end   ();}

    double Image::eval (const dvec3 & p) const
    {
      int dx = this->dx ();
      int dy = this->dy ();
      int dz = this->dz ();

      double x = (p[0] * dx) - 0.5, u = floor (x); int int_u = (int) u;
      double y = (p[1] * dy) - 0.5, v = floor (y); int int_v = (int) v;
      double z = (p[2] * dz) - 0.5, w = floor (z); int int_w = (int) w;

      // 8 bits for 8 corners.
      AaUInt8 f = 0xFF;
      if (int_u + 1 <   0) f = 0x00; else if (int_u     <   0) f &= (0x02 | 0x08 | 0x20 | 0x80);
      if (int_u     >= dx) f = 0x00; else if (int_u + 1 >= dx) f &= (0x01 | 0x04 | 0x10 | 0x40);
      if (int_v + 1 <   0) f = 0x00; else if (int_v     <   0) f &= (0x04 | 0x08 | 0x40 | 0x80);
      if (int_v     >= dy) f = 0x00; else if (int_v + 1 >= dy) f &= (0x01 | 0x02 | 0x10 | 0x20);
      if (int_w + 1 <   0) f = 0x00; else if (int_w     <   0) f &= (0x10 | 0x20 | 0x40 | 0x80);
      if (int_w     >= dz) f = 0x00; else if (int_w + 1 >= dz) f &= (0x01 | 0x02 | 0x04 | 0x08);

      const Mono8::Pixel * d = m_image.begin () + ((int_w * dy + int_v) * dx + int_u); 
      return TriLinear (x - u, y - v, z - w,
                        (f & 0x01) ? d[             0][0] : 0,
                        (f & 0x02) ? d[           + 1][0] : 0,
                        (f & 0x04) ? d[      + dx    ][0] : 0,
                        (f & 0x08) ? d[      + dx + 1][0] : 0,
                        (f & 0x10) ? d[dx*dy         ][0] : 0,
                        (f & 0x20) ? d[dx*dy      + 1][0] : 0,
                        (f & 0x40) ? d[dx*dy + dx    ][0] : 0,
                        (f & 0x80) ? d[dx*dy + dx + 1][0] : 0);
    }

    GLuint Image::glTex3d () const
      throw (Aa::GL::MissingExtension)
    {
      GLuint id = 0;
      glGenTextures (1, &id);
      glBindTexture (GL_TEXTURE_3D, id);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

      if (! GLEW_ARB_texture_non_power_of_two)
        throw GL::MissingExtension ("GL_ARB_texture_non_power_of_two");

      glTexImage3D (GL_TEXTURE_3D, 0,
                    GL_R8, this->dx (), this->dy (), this->dz (), 0,
                    GL_RED, GL_UNSIGNED_BYTE, this->begin ());

      glGenerateMipmap (GL_TEXTURE_3D);

      return id;
    }

    void ImageLoadB8 (Image        * image,
                      const string & filename,
                      const vec3   & scale)
      throw (Aa::FileNotFound, Aa::ParseError)
    {
      cout << "ImageLoadB8 (\"" << filename << "\");" << endl;
      ifstream f (filename.c_str (), ios::in | ios::binary);
      if (! f.is_open ()) throw FileNotFound (filename);
      // Read dimensions.
      AaUInt16 dx = 0;
      if (! f.read ((char *) &dx, sizeof (dx))) throw Aa::ParseError ("dx");
      AaUInt16 dy = 0;
      if (! f.read ((char *) &dy, sizeof (dy))) throw Aa::ParseError ("dy");
      AaUInt16 dz = 0;
      if (! f.read ((char *) &dz, sizeof (dz))) throw Aa::ParseError ("dz");
      // Resize buffer.
      image->setScale (scale);
      image->setPosition (vec3 ());
      image->setOrientation (mat3 ());
      image->resize (vec<AaUInt> (dx, dy, dz));
      // Read data.
      if (! f.read ((char *) image->begin (), dx * dy * dz)) throw Aa::ParseError ("dat");
    }

    Image * ImageLoadB8 (const string & filename, const vec3 & scale)
      throw (Aa::FileNotFound, Aa::ParseError)
    {
      Image * image = new Image (uvec3 (0u));
      try {ImageLoadB8 (image, filename, scale);} catch (...) {delete image; throw;}
      return image;
    }
  } // namespace R3d
} // namespace Aa

