#ifndef R3D_IMAGE_RENDERER_3D_GLSL__H
#define R3D_IMAGE_RENDERER_3D_GLSL__H

#include "R3dImageRenderer3d.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API ImageRenderer3dGLSL :
      public ImageRenderer3d
    {
      protected:
        GLSL::Program         m_program;
        GLSL::Location<mat4>  m_program_modelview;
        GLSL::Location<mat4>  m_program_projection;
        GLSL::Location<int>   m_program_tex3d;
        GLSL::Location<float> m_program_min;
        GLSL::Location<float> m_program_max;
        
      protected:
        virtual void glPreDraw  (const GL::CoreContext &);
        virtual void glPostDraw (const GL::CoreContext &);

      public:
        ImageRenderer3dGLSL (const std::string & fragment = "/Aa/R3d/Texture.fragment");
        virtual ~ImageRenderer3dGLSL ();
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_IMAGE_RENDERER_3D_GLSL__H

