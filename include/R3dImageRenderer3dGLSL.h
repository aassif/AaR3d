#ifndef __R3D_IMAGE_RENDERER_3D_GLSL__
#define __R3D_IMAGE_RENDERER_3D_GLSL__

#include <AaGLSL>
#include "R3dImageRenderer3d.h"

namespace Aa
{
  namespace R3d
  {
    class ImageRenderer3dGLSL : public ImageRenderer3d
    {
      protected:
        int m_mode;
        GL::Program m_program;
        GLuint m_table;
        bool m_blinn;
        float m_shininess;
        Math::vR3 m_delta;
        
      protected:
        virtual void glSlice_ViewAligned_Cube (bool) const;

      public:
        ImageRenderer3dGLSL (const Image * = NULL, const Lut * = NULL);
        virtual ~ImageRenderer3dGLSL ();
        int mode () const {return m_mode;}
        void setMode (int m) {m_mode = m;}
        bool lighting () const {return m_blinn;}
        void setLighting (bool b) {m_blinn = b;}
        virtual void glPreDraw (bool = false) const;
        virtual void glPostDraw (bool = false) const;
    };
  } // namespace R3d
} // namespace Aa

#endif // __R3D_IMAGE_RENDERER_3D_GLSL__

