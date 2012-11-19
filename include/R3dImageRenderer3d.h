#ifndef __AA_R3D_IMAGE_RENDERER_3D__
#define __AA_R3D_IMAGE_RENDERER_3D__

#include "R3dSlicer.h"
#include "R3dImageRenderer.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API ImageRenderer3d : public ImageRenderer
    {
      protected:
        Slicer * m_slicer;
        box3     m_box;
        GLuint   m_image_tex3d;
        vec3     m_image_scale;
        GLfloat  m_steps [2];

      protected:
        virtual void glPreDraw  (bool);
        virtual void glPostDraw (bool);

      public:
        ImageRenderer3d ();
        virtual ~ImageRenderer3d ();
        virtual void setImg (const Image * = NULL);
        virtual void setImg (const box3 &, GLuint tex3d, const vec3 & scale);
        virtual void setLut (const Lut * = NULL);
        virtual void glDraw (bool = false);
        // Step.
        double getStep (bool motion) const;
        void setStep (bool motion, double);
    };
  } // namespace R3d
} // namespace Aa

#endif // __AA_R3D_IMAGE_RENDERER_3D__

