#ifndef R3D_POST_CLASSIFICATION__H
#define R3D_POST_CLASSIFICATION__H

#include <AaGLSL>
#include "R3dImageRenderer3dGLSL.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API PostClassification :
      public ImageRenderer3dGLSL
    {
      protected:
        GLuint m_lut_tex1d [2];
        
      protected:
        virtual void glPreDraw  (bool);
        virtual void glPostDraw (bool);

      public:
        PostClassification (const std::string & vertex,
                            const std::string & geometry,
                            const std::string & fragment);
        virtual ~PostClassification ();
        virtual void setLut (const Lut * = NULL);
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_POST_CLASSIFICATION__H

