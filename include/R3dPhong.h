#ifndef R3D_PHONG__H
#define R3D_PHONG__H

#include "R3dPostClassification.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Phong :
      public PostClassification
    {
      protected:
        float m_ambient;
        float m_diffuse;
        float m_specular;
        float m_shininess;
        
      protected:
        virtual void glPreDraw (const GL::CoreContext &);

      public:
        Phong (const std::string & fragment = "/Aa/R3d/Phong.fragment");
        virtual ~Phong ();

        float ambient () const {return m_ambient;}
        void setAmbient (float f) {m_ambient = f;}

        float diffuse () const {return m_diffuse;}
        void setDiffuse (float f) {m_diffuse = f;}

        float specular () const {return m_specular;}
        void setSpecular (float f) {m_specular = f;}

        float shininess () const {return m_shininess;}
        void setShininess (float f) {m_shininess = f;}
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_PHONG__H

