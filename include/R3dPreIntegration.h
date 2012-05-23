#ifndef R3D_PRE_INTEGRATION__H
#define R3D_PRE_INTEGRATION__H

#include <AaGLSL>
#include "R3dImageRenderer3dGLSL.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API PreIntegration :
      public ImageRenderer3dGLSL
    {
      protected:
        GLuint m_lut_tex2d [2];
        
      protected:
        virtual void glPreDraw  (bool);
        virtual void glPostDraw (bool);

      public:
        PreIntegration (const std::string & vertex,
                        const std::string & geometry,
                        const std::string & fragment);
        virtual ~PreIntegration ();
        virtual void setLut (const Lut * = NULL);
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_PRE_INTEGRATION__H

