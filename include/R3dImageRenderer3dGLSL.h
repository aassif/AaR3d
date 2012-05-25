#ifndef R3D_IMAGE_RENDERER_3D_GLSL__H
#define R3D_IMAGE_RENDERER_3D_GLSL__H

#include <AaGLSL>
#include "R3dImageRenderer3d.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API ImageRenderer3dGLSL :
      public ImageRenderer3d
    {
      protected:
        GL::Program m_program;
        GLfloat m_min;
        GLfloat m_max;
        
      protected:
        virtual void glPreDraw  (bool);
        virtual void glPostDraw (bool);

      public:
        ImageRenderer3dGLSL (const std::string & vertex,
                             const std::string & geometry,
                             const std::string & fragment);
        virtual ~ImageRenderer3dGLSL ();
        void setWindow (GLfloat min, GLfloat max);
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_IMAGE_RENDERER_3D_GLSL__H

