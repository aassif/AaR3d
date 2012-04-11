#ifndef __R3D_IMAGE_RENDERER_3D_GLSL__
#define __R3D_IMAGE_RENDERER_3D_GLSL__

#include <AaGLSL>
#include "R3dImageRenderer3d.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API ImageRenderer3dGLSL : public ImageRenderer3d
    {
      protected:
        int m_mode;
        GL::Program m_program;
        GLuint m_table_tex2d;
        bool m_blinn;
        float m_shininess;
        vec3 m_delta;
        
      protected:
        virtual void glSlice_ViewAligned_Cube (bool) const;

      public:
        ImageRenderer3dGLSL (const Image * = NULL, const Lut * = NULL);
        virtual ~ImageRenderer3dGLSL ();
        int mode () const {return m_mode;}
        void setMode (int m) {m_mode = m;}
        bool lighting () const {return m_blinn;}
        void setLighting (bool b) {m_blinn = b;}
        float shininess () const {return m_shininess;}
        void setShininess (float f) {m_shininess = f;}
        virtual void glPreDraw (bool = false) const;
        virtual void glPostDraw (bool = false) const;
    };
  } // namespace R3d
} // namespace Aa

#endif // __R3D_IMAGE_RENDERER_3D_GLSL__

