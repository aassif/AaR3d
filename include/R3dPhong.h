#ifndef R3D_BLINN__H
#define R3D_BLINN__H

#include "R3dPostClassification.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Phong :
      public PostClassification
    {
      protected:
        float m_shininess;
        vec3  m_delta;
        
      protected:
        virtual void glPreDraw (const GL::CoreContext &);

      public:
        Phong (const std::string & fragment = "/Aa/R3d/Phong.fragment");
        virtual ~Phong ();
        float shininess () const {return m_shininess;}
        void setShininess (float f) {m_shininess = f;}
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_BLINN__H

