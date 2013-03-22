#ifndef AA_R3D_IMAGE_RENDERER__H
#define AA_R3D_IMAGE_RENDERER__H

#include <AaCoreContext>
#include "R3dLut.h"
#include "R3dImage.h"

namespace Aa
{
  namespace R3d
  {
    // ImageRenderer.

    class ImageRenderer
    {
      public:
        class ClippingPlane
        {
          public:
            bool active;
            vec3 point;
            vec3 normal;

          public:
            ClippingPlane (const vec3 & p, const vec3 & n);
            bool test (const vec3 &) const;
        };

      public:
        std::vector<ClippingPlane> planes;
        GLfloat m_min;
        GLfloat m_max;

      protected:
        virtual void glPreDraw  (const GL::CoreContext &) = 0;
        virtual void glPostDraw (const GL::CoreContext &) = 0;

      public:
        ImageRenderer ();
        virtual ~ImageRenderer ();
        void setRange (GLfloat min, GLfloat max);
        virtual void setImage (const Image *) = 0;
        virtual void setLut (const Lut *) = 0;
        virtual void glDraw (const GL::CoreContext &) = 0;
    };
  } // namespace R3d
} // namespace Aa

#endif // AA_R3D_IMAGE_RENDERER__H

