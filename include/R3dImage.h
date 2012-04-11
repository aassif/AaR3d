#ifndef __AA_R3D_IMAGE__
#define __AA_R3D_IMAGE__

#include <string>
#include <AaMath>
#include <AaImage>
#include "AaR3d.h"

// Image.

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Image
    {
      public:
        inline static
        double Linear (double du,
                       double u0, double u1)
        {
          return u0 + du * (u1 - u0);
        }

        inline static
        double BiLinear (double du, double dv,
                         double u0, double u1, double v0, double v1)
        {
          return Linear (dv,
                         Linear (du, u0, u1),
                         Linear (du, v0, v1));
        }

        inline static
        double TriLinear (double du, double dv, double dw,
                          double d0, double d1, double d2, double d3,
                          double d4, double d5, double d6, double d7)
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
        dbox3        m_box;
#endif

      public:
        // Constructor.
        Image (const uvec3 &);
        // Destructor.
        virtual ~Image ();
        // Reallocation.
        void resize (const uvec3 &);
        // Dimensions.
        unsigned int dx () const;
        unsigned int dy () const;
        unsigned int dz () const;
        // Bounding box.
        void setBox (const dbox3 &);
        const dbox3 & box () const;
        // Iterators.
        /***/ Mono8::Pixel * begin ();
        const Mono8::Pixel * begin () const;
        /***/ Mono8::Pixel * end ();
        const Mono8::Pixel * end () const;
        // Potential.
        double eval (const dvec3 &) const;
    };

    // Loading.
    AA_R3D_API void ImageLoadB8 (Image *, const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    AA_R3D_API Image * ImageLoadB8 (const std::string &)
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

#endif // __AA_R3D_IMAGE__

