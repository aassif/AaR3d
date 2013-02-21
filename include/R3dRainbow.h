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
        vec3 m_delta;
        
      protected:
        virtual void glPreDraw (const GL::CoreContext &);

      public:
        Rainbow ();
        virtual ~Rainbow ();
    };

  } // namespace R3d
} // namespace Aa

#endif // R3D_RAINBOW__H

