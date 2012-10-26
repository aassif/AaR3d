#ifndef R3D_BLINN__H
#define R3D_BLINN__H

#include "R3dPostClassification.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Blinn :
      public PostClassification
    {
      protected:
        float m_shininess;
        vec3  m_delta;
        
      protected:
        virtual void glPreDraw (bool);

      public:
#if 0
        Blinn (const std::string & vertex,
               const std::string & geometry,
               const std::string & fragment);
#endif
        Blinn (const std::string & fragment = "/AaR3d/Blinn.fragment");
        virtual ~Blinn ();
        float shininess () const {return m_shininess;}
        void setShininess (float f) {m_shininess = f;}
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_BLINN__H

