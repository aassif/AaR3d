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
      Im<3, Mono8> (dims),
      m_scale (scale),
      m_position (position),
      m_orientation (orientation)
    {
    }

    Image::~Image ()
    {
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

    GLuint Image::glTex3d () const
      throw (Aa::GL::MissingExtension)
    {
#ifdef __APPLE__
      throw GL::MissingExtension ("GL_ARB_texture_non_power_of_two");
#else
      if (! GLEW_ARB_texture_non_power_of_two)
        throw GL::MissingExtension ("GL_ARB_texture_non_power_of_two");
#endif

      GLuint id = 0;
      glGenTextures (1, &id);
      glBindTexture (GL_TEXTURE_3D, id);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

      const uvec3 & d = this->dims ();
      glTexImage3D (GL_TEXTURE_3D, 0,
                    GL_R8, d[0], d[1], d[2], 0,
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
      image->alloc (vec<AaUInt> (dx, dy, dz));
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

