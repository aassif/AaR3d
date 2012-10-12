#ifndef R3D_RAINBOW__H
#define R3D_RAINBOW__H

#include "R3dPostClassification.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Rainbow :
      public PostClassification
    {
      protected:
        vec3  m_delta;
        
      protected:
        virtual void glPreDraw (bool);

      public:
#if 0
        Rainbow (const std::string & vertex,
                 const std::string & geometry,
                 const std::string & fragment);
#endif
        Rainbow ();
        virtual ~Rainbow ();
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_RAINBOW__H

