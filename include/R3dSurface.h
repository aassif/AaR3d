#ifndef R3D_SURFACE__H
#define R3D_SURFACE__H

#include <AaGLSL>
#include "R3dImageRenderer3dGLSL.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Surface :
      public ImageRenderer3dGLSL
    {
      protected:
        float  m_iso;
        vec4   m_color;
        GLuint m_lut_tex2d;
        
      protected:
        virtual void glPreDraw  (const GL::CoreContext &);

      public:
        Surface (const std::string & fragment = "/Aa/R3d/Surface.fragment");
        virtual ~Surface ();
        virtual void setLut (const Lut * = NULL);
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_SURFACE__H

