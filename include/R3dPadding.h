#ifndef AA_R3D_IMAGE_PADDING__H
#define AA_R3D_IMAGE_PADDING__H

#include "AaR3d.h"

// Image.

namespace Aa
{
  namespace R3d
  {

    unsigned char * pad3dxyz (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src,
                              unsigned short dx, unsigned short dy, unsigned short dz);

    unsigned char * pad3d_yz (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src,
                              unsigned short dy, unsigned short dz);

    unsigned char * pad3d__z (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src,
                              unsigned short dz);

    unsigned char * pad3d___ (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src);

    unsigned char * pad3d (unsigned short sx, unsigned short sy, unsigned short sz,
                           const unsigned char * src,
                           unsigned short dx, unsigned short dy, unsigned short dz);

  } // namespace R3d
} // namespace Aa

#endif // AA_R3D_IMAGE_PADDING__H

