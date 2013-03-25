#ifndef AA_R3D_IMAGE__H
#define AA_R3D_IMAGE__H

#include <GL/glew.h>
//#include <string>
#include <AaBox>
#include <AaImage>
#include <AaGL>
#include "AaR3d.h"

// Image.

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Image :
      public Im<3, Mono8>
    {
      protected:
        vec3         m_scale;
        vec3         m_position;
        mat3         m_orientation;

      public:
        // Constructor.
        Image (const uvec3 & dims,
               const vec3 & scale       = vec3 (1.0f),
               const vec3 & position    = vec3 (0.0f),
               const mat3 & orientation = mat3 ());
        // Destructor.
        virtual ~Image ();
        // Scale.
        void setScale (const vec3 &);
        const vec3 & scale () const {return m_scale;}
        // Position.
        void setPosition (const vec3 &);
        const vec3 & position () const {return m_position;}
        // Orientation.
        void setOrientation (const mat3 &);
        const mat3 & orientation () const {return m_orientation;}
        // Transform.
        mat4 transform () const;
        // OpenGL texture.
        GLuint glTex3d () const throw (Aa::GL::MissingExtension);
    };

    // Loading.
    AA_R3D_API void ImageLoadB8 (Image *, const std::string &, const vec3 & scale = vec3 (1))
      throw (Aa::FileNotFound, Aa::ParseError);

    AA_R3D_API Image * ImageLoadB8 (const std::string &, const vec3 & scale = vec3 (1))
      throw (Aa::FileNotFound, Aa::ParseError);

#if 0
    AA_R3D_API void ImageLoadPIC (Image *, const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    AA_R3D_API Image * ImageLoadPIC (const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    AA_R3D_API void ImageLoadDICOM (Image *, const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    AA_R3D_API Image * ImageLoadDICOM (const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);
#endif

  } // namespace R3d
} // namespace Aa

#endif // AA_R3D_IMAGE__H

