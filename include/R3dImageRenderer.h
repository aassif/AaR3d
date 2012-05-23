#ifndef __AA_R3D_IMAGE_RENDERER__
#define __AA_R3D_IMAGE_RENDERER__

#include <AaScene>
#include "R3dLut.h"
#include "R3dImage.h"

namespace Aa
{
  namespace R3d
  {
    // ImageRenderer.

    class ImageRenderer : public Aa::GL::Plugin
    {
      public:
        class ClippingPlane
        {
          public:
            bool  active;
            dvec3 point;
            dvec3 normal;

          public:
            ClippingPlane (const dvec3 & p, const dvec3 & n);
            bool test (const dvec3 &) const;
        };

      public:
        std::vector<ClippingPlane> planes;

      protected:
        virtual void glPreDraw (bool) = 0;
        virtual void glPostDraw (bool) = 0;

      public:
        ImageRenderer ();
        virtual ~ImageRenderer ();
        virtual void setImg (const Image *) = 0;
        virtual void setLut (const Lut *) = 0;
        virtual void glDraw (bool = false) = 0;
    };
  } // namespace R3d
} // namespace Aa

#endif // __AA_R3D_IMAGE_RENDERER__

