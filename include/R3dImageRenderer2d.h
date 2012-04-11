#ifndef __AA_R3D_IMAGE_RENDERER_2D__
#define __AA_R3D_IMAGE_RENDERER_2D__

#include "R3dImageRenderer.h"

namespace Aa
{
  namespace R3d
  {
    // ImageRenderer2d

    class AA_R3D_API ImageRenderer2d : public ImageRenderer
    {
      protected:
        unsigned int m_dz;
        GLuint *     m_img_textures;
        GLdouble     m_img_scales [2];
        GLubyte      m_lut [1024];
        dbox3        m_box;
     
      public:
        ImageRenderer2d (const Image * = NULL, const Lut * = NULL);
        virtual ~ImageRenderer2d ();
        virtual void setImg (const Image * = NULL);
        virtual void setLut (const Lut * = NULL);
        virtual void glDraw (bool = false);
    };
  } // namespace R3d
} // namespace Aa

#endif // __AA_R3D_IMAGE_RENDERER_2D__

