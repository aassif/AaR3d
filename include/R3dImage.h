#ifndef AA_R3D_IMAGE__H
#define AA_R3D_IMAGE__H

#include <GL/glew.h>
//#include <string>
#include <AaMath>
#include <AaImage>
#include <AaGL>
#include "AaR3d.h"

// Image.

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Image
    {
      public:
        template <class T>
        inline static
        AA_PROMOTE(T,double) Linear (double du,
                                     const T & u0, const T & u1)
        {
          return (1.0 - du) * u0 + du * u1;
        }

        template <class T>
        inline static
        AA_PROMOTE(T,double) BiLinear (double du, double dv,
                                       const T & u0, const T & u1,
                                       const T & v0, const T & v1)
        {
          return Linear (dv,
                         Linear (du, u0, u1),
                         Linear (du, v0, v1));
        }

        template <class T>
        inline static
        AA_PROMOTE(T,double) TriLinear (double du, double dv, double dw,
                                        const T & d0, const T & d1,
                                        const T & d2, const T & d3,
                                        const T & d4, const T & d5,
                                        const T & d6, const T & d7)
        {
          return Linear (dw,
                         BiLinear (du, dv, d0, d1, d2, d3),
                         BiLinear (du, dv, d4, d5, d6, d7));
        }

#if 0
      public:
        typedef enum
        {
          RUNTIME_DEFAULT =  0,
          RUNTIME_MONO8   =  1,
          RUNTIME_MONO16  =  2,
          //RUNTIME_MONOf   =  3,
          //RUNTIME_MONOd   =  4,
          RUNTIME_RGB8    =  5,
          //RUNTIME_RGB16   =  6,
          //RUNTIME_RGBf    =  7,
          //RUNTIME_RGBd    =  8,
          RUNTIME_RGBA8   =  9,
          //RUNTIME_RGBA16  = 10,
          //RUNTIME_RGBAf   = 11,
          //RUNTIME_RGBAd   = 12,
          RUNTIME_YCbCr8  = 13,
          //RUNTIME_YCbCr16 = 14,
          //RUNTIME_YCbCrf  = 15,
          //RUNTIME_YCbCrd  = 16
        }
        RuntimeType;

      protected:
        RuntimeType m_type;
        void      * m_image;
        dbox3       m_box;
#else
      protected:
        Im<3, Mono8> m_image;
        vec3         m_scale;
        vec3         m_position;
        mat3         m_orientation;
#endif

      public:
        // Constructor.
        Image (const uvec3 & dims,
               const vec3 & scale       = vec3 (1.0f),
               const vec3 & position    = vec3 (0.0f),
               const mat3 & orientation = mat3 ());
        // Destructor.
        virtual ~Image ();
        // Reallocation.
        void resize (const uvec3 &);
        // Dimensions.
        uvec3  dims () const {return m_image.dims ();}
        AaUInt dx   () const {return m_image.dim (0);}
        AaUInt dy   () const {return m_image.dim (1);}
        AaUInt dz   () const {return m_image.dim (2);}
        // Scale.
        void setScale (const vec3 &);
        const vec3 & scale () const {return m_scale;}
        // Position.
        void setPosition (const vec3 &);
        const vec3 & position () const {return m_position;}
        // Orientation.
        void setOrientation (const mat3 &);
        const mat3 & orientation () const {return m_orientation;}
        // Bounding box.
        //void setBox (const box3 &);
        //const box3 & box () const {return m_box;}
        // Transform.
        mat4 transform () const;
        // Iterators.
        /***/ Mono8::Pixel * begin ();
        const Mono8::Pixel * begin () const;
        /***/ Mono8::Pixel * end   ();
        const Mono8::Pixel * end   () const;
        // Potential.
        double eval (const dvec3 &) const;
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

