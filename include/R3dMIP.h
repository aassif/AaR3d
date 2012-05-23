#ifndef R3D_MIP__H
#define R3D_MIP__H

#include "R3dImageRenderer3dGLSL.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API MIP :
      public ImageRenderer3dGLSL
    {
      protected:
        GLenum m_mode;
        
      protected:
        virtual void glPreDraw  (bool);
        virtual void glPostDraw (bool);

      public:
        MIP (const std::string & vertex,
             const std::string & geometry,
             const std::string & fragment,
             GLenum mode);
        virtual ~MIP ();
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_MIP__H

