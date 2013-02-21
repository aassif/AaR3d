#ifndef AA_R3D_IMAGE_RENDERER_3D__H
#define AA_R3D_IMAGE_RENDERER_3D__H

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
        GLuint   m_tex3d;
        bool     m_tex3d_shared;
        GLfloat  m_steps [2];

      protected:
        virtual void glPreDraw  (const GL::CoreContext &);
        virtual void glPostDraw (const GL::CoreContext &);

      public:
        ImageRenderer3d ();
        virtual ~ImageRenderer3d ();
        virtual void setImage (const Image * = NULL);
        virtual void setImage (GLuint tex3d, bool shared = true);
        virtual void setLut (const Lut * = NULL);
        virtual void glDraw (const GL::CoreContext &);
        // Step.
        double step (bool motion) const;
        void setStep (bool motion, double);
    };
  } // namespace R3d
} // namespace Aa

#endif // AA_R3D_IMAGE_RENDERER_3D__H

