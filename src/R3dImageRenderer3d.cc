#include <fstream>
#include <cmath>
#include <vector> // ImageRenderer3d::glSlice_ViewAligned_Naive
#include <GL/glew.h>
#include <AaText>
#include <AaPrimitives>
#include "R3dImageRenderer3d.h"

using namespace std;
using namespace Aa::Math;

#ifdef DEBUG
#define ASSERT(x) assert (x)
#else
#define ASSERT(x)
#endif

namespace Aa
{
  namespace R3d
  {
    /*
    ** ImageRenderer3d.
    */

    //         X
    //         |
    //         |
    //         +---- Y
    //        /
    //       Z

    // based on the mc table designed
    // by Paul Bourke and Cory Gene Bloyd.

    const ImageRenderer3d::EdgeIndex ImageRenderer3d::TABLE [256][8] =
    {
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  8,  0, -1, -1, -1, -1, -1},
      { 0, 10,  6, -1, -1, -1, -1, -1},
      { 4,  8,  6, 10, -1, -1, -1, -1},
      { 4,  2,  9, -1, -1, -1, -1, -1},
      { 2,  9,  0,  8, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  9,  6,  8, 10, -1, -1, -1},
      { 2,  6, 11, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 0, 10,  2, 11, -1, -1, -1, -1},
      { 4,  8,  2, 10, 11, -1, -1, -1},
      { 9,  4, 11,  6, -1, -1, -1, -1},
      { 6, 11,  0,  9,  8, -1, -1, -1},
      { 0, 10,  4, 11,  9, -1, -1, -1},
      { 9,  8, 11, 10, -1, -1, -1, -1},
      { 5,  1,  8, -1, -1, -1, -1, -1},
      { 5,  1,  4,  0, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  6,  1,  4,  5, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  1,  9,  0,  2, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  2, 10,  9,  1,  5, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  4, 11,  5, 10,  1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  1,  9,  0, 11,  6, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  9, 10,  5,  1, -1, -1, -1},
      { 1,  7, 10, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  7,  0,  6, -1, -1, -1, -1},
      { 1,  7,  8,  6,  4, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  9,  6,  8,  7,  1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  2,  7,  0,  1, -1, -1, -1},
      {11,  2,  7,  4,  1,  8, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7, 11,  1,  9,  0,  4, -1, -1},
      { 9,  8, 11,  1,  7, -1, -1, -1},
      { 7, 10,  5,  8, -1, -1, -1, -1},
      { 0,  4, 10,  5,  7, -1, -1, -1},
      { 8,  5,  0,  7,  6, -1, -1, -1},
      { 4,  5,  6,  7, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  0,  7,  2,  5,  9, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7,  6,  5,  2,  9, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 8,  5,  0,  7,  2, 11, -1, -1},
      { 5,  7,  4, 11,  2, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7, 11,  5,  9, -1, -1, -1, -1},
      { 9,  3,  5, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  2,  5,  3, -1, -1, -1, -1},
      { 8,  0,  5,  2,  3, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  8,  3, 10,  2,  6, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 3,  5, 11,  4,  6, -1, -1, -1},
      { 5,  8,  3,  0, 11,  6, -1, -1},
      { 0, 10,  4, 11,  5,  3, -1, -1},
      {10, 11,  8,  3,  5, -1, -1, -1},
      { 9,  3,  8,  1, -1, -1, -1, -1},
      { 9,  3,  4,  1,  0, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  6,  1,  4,  3,  9, -1, -1},
      { 4,  2,  8,  3,  1, -1, -1, -1},
      { 2,  3,  0,  1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 3,  1,  2, 10,  6, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  3,  6,  1,  4,  8, -1, -1},
      { 1,  0,  3,  6, 11, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1, 10,  3, 11, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7, 10,  3,  8,  9, -1, -1, -1},
      { 0,  4, 10,  9,  7,  3, -1, -1},
      { 3,  7,  9,  6,  8,  0, -1, -1},
      { 6,  4,  7,  9,  3, -1, -1, -1},
      { 4,  2,  8,  3, 10,  7, -1, -1},
      { 2,  3,  0,  7, 10, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  2,  7,  3, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  3,  7, -1, -1, -1, -1, -1},
      {11,  7,  3, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  6,  3,  7, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7,  3, 10,  2,  0, -1, -1, -1},
      { 7,  3, 10,  2,  8,  4, -1, -1},
      { 9,  4,  3,  6,  7, -1, -1, -1},
      { 0,  6,  8,  7,  9,  3, -1, -1},
      { 3,  9,  7,  4, 10,  0, -1, -1},
      { 8, 10,  9,  7,  3, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  1, 11,  3, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  0, 11,  1,  3, -1, -1, -1},
      { 8,  1,  4,  3,  6, 11, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  1,  6,  3,  2, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  0,  3,  1, -1, -1, -1, -1},
      { 3,  2,  1,  4,  8, -1, -1, -1},
      { 9,  4,  3,  6,  1, 10, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  3,  0,  9,  4, -1, -1, -1},
      { 3,  9,  1,  8, -1, -1, -1, -1},
      { 3, 11,  5, 10,  8, -1, -1, -1},
      { 3, 11,  5, 10,  4,  0, -1, -1},
      { 6,  0, 11,  8,  3,  5, -1, -1},
      { 4,  5,  6,  3, 11, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6, 10,  2,  8,  3,  5, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  0,  3,  8,  5, -1, -1, -1},
      { 2,  4,  3,  5, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 9,  5,  3, -1, -1, -1, -1, -1},
      {11,  7,  9,  5, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  7,  2,  5,  4, -1, -1, -1},
      {11,  7,  2,  5,  0,  8, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  6,  9,  7,  5, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 9,  2,  5,  0,  7, 10, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  6,  5,  7, -1, -1, -1, -1},
      { 7,  5,  6,  8,  0, -1, -1, -1},
      { 5,  4,  7,  0, 10, -1, -1, -1},
      {10,  7,  8,  5, -1, -1, -1, -1},
      { 1,  8,  7,  9, 11, -1, -1, -1},
      { 4,  9,  0, 11,  1,  7, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 8,  4,  1,  2,  7, 11, -1, -1},
      { 0,  2,  1, 11,  7, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  8,  7,  9,  6,  2, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  7,  4,  1,  8, -1, -1, -1},
      { 7,  1,  6,  0, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1, 10,  7, -1, -1, -1, -1, -1},
      { 5,  9,  1, 11, 10, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  0, 11,  1,  9,  5, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  5, 10,  4, 11,  2, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  9,  1,  2, 10,  6, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 0,  1,  2,  5,  9, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  6,  5, 10,  1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  5,  0,  4, -1, -1, -1, -1},
      { 5,  8,  1, -1, -1, -1, -1, -1},
      { 9, 11,  8, 10, -1, -1, -1, -1},
      {11, 10,  9,  0,  4, -1, -1, -1},
      { 9, 11,  8,  6,  0, -1, -1, -1},
      { 4,  9,  6, 11, -1, -1, -1, -1},
      {10,  8, 11,  4,  2, -1, -1, -1},
      {10,  0, 11,  2, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2, 11,  6, -1, -1, -1, -1, -1},
      { 8,  9, 10,  2,  6, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 9,  2,  8,  0, -1, -1, -1, -1},
      { 4,  9,  2, -1, -1, -1, -1, -1},
      { 8,  4, 10,  6, -1, -1, -1, -1},
      { 0,  6, 10, -1, -1, -1, -1, -1},
      { 4,  0,  8, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1}
    };

    unsigned char * padImage3dxyz (unsigned short sx, unsigned short sy, unsigned short sz,
                                   const unsigned char * src,
                                   unsigned short dx, unsigned short dy, unsigned short dz)
    {
#if 0
      cout << "padImage3dxyz ("
           << "sx = " << sx << ", sy = " << sy << ", sz = " << sz << ", "
           << "src = " << (void *) src << ", "
           << "dx = " << dx << ", dy = " << dy << ", dz = " << dz << ")\n";
#endif
      // Aliases.
      unsigned long dxdy = dx * dy;
      unsigned long dxdydz = dxdy * dz;
      // Memory allocation.
      unsigned char * dst = new unsigned char [dxdydz];
      // Iterators.
      const unsigned char * s = src;
      unsigned char * d = dst;
#if 0
      // The 2 loops (fill with zeros).
      unsigned short vy = dy - sy;
      fill (dst, dst + dxdydz, 0);
      unsigned long dxvy = dx * vy;
      for (unsigned short z = sz; z--; d += dxvy)
        for (unsigned short y = sy; y--; d += dx) copy (s, s += sx, d);
#else
      // The 2 loops (copy border).
      unsigned short vx = dx - sx;
      unsigned short vy = dy - sy;
      unsigned short vz = dz - sz;
      for (unsigned short z = sz; z--;)
      {
        for (unsigned short y = sy; y--;)
        {
          d = copy (s, s + sx, d); s += sx;
          unsigned char c = d [-1]; // crash if sx < 1
          fill (d, d + vx, c); d += vx;
        }
        for (unsigned short y = vy; y--;)
          d = copy (d - dx, d, d); // crash if sy < 1
      }
      for (unsigned short z = vz; z--;)
        d = copy (d - dxdy, d, d); // crash if sz < 1
#endif
      return dst;
    }

    unsigned char * padImage3d_yz (unsigned short sx, unsigned short sy, unsigned short sz,
                                   const unsigned char * src,
                                   unsigned short dy, unsigned short dz)
    {
#if 0
      cout << "padImage3d_yz ("
           << "sx = " << sx << ", sy = " << sy << ", sz = " << sz << ", "
           << "src = " << (void *) src << ", "
           << "dy = " << dy << ", dz = " << dz << ")\n";
#endif
      // Aliases.
      unsigned long sxsy = sx * sy;
      unsigned long sxdy = sx * dy;
      unsigned long sxdydz = sxdy * dz;
      // Memory allocation.
      unsigned char * dst = new unsigned char [sxdydz];
      // Iterators.
      const unsigned char * s = src;
      unsigned char * d = dst;
#if 0
      // The loop (fill with zeros).
      fill (dst, dst + sxdydz, 0);
      for (unsigned short z = sz; z--; d += sxdy) copy (s, s += sxsy, d);
#else
      unsigned short vy = dy - sy;
      unsigned short vz = dz - sz;
      // The loop (copy border).
      for (unsigned short z = sz; z--;)
      {
        d = copy (s, s + sxsy, d); s += sxsy;
        for (unsigned short y = vy; y--;)
          d = copy (d - sx, d, d); // crash if sy < 1
      }
      for (unsigned short z = vz; z--;)
        d = copy (d - sxdy, d, d); // crash if sz < 1
#endif
      return dst;
    }

    unsigned char * padImage3d__z (unsigned short sx, unsigned short sy, unsigned short sz,
                                   const unsigned char * src,
                                   unsigned short dz)
    {
#if 0
      cout << "padImage3d__z ("
           << "sx = " << sx << ", sy = " << sy << ", sz = " << sz << ", "
           << "src = " << (void *) src << ", "
           << "dz = " << dz << ")\n";
#endif
      // Aliases.
      unsigned long sxsy = sx * sy;
      unsigned long sxsydz = sxsy * dz;
      // Memory allocation.
      unsigned char * dst = new unsigned char [sxsydz];
#if 0
      // Simple copy.
      fill (dst, dst + sxsydz, 0);
      copy (src, src + sxsy * sz, dst);
#else
      // Simple copy.
      unsigned char * d = dst;
      unsigned short vz = dz - sz;
      d = copy (src, src + sxsy * sz, d);
      for (unsigned short z = vz; z--;)
        d = copy (d - sxsy, d, d); // crash if sz < 1
#endif
      return dst;
    }

    unsigned char * padImage3d___ (unsigned short sx, unsigned short sy, unsigned short sz,
                                   const unsigned char * src)
    {
#if 0
      cout << "padImage3d___ ("
           << "sx = " << sx << ", sy = " << sy << ", sz = " << sz << ", "
           << "src = " << (void *) src << ")\n";
#endif
      unsigned long sxsysz = sx * sy * sz;
      unsigned char * dst = new unsigned char [sxsysz];
      copy (src, src + sxsysz, dst);
      return dst;
    }

    unsigned char * padImage3d (unsigned short sx, unsigned short sy, unsigned short sz,
                                const unsigned char * src,
                                unsigned short dx, unsigned short dy, unsigned short dz)
    {
      if (dx != sx) return padImage3dxyz (sx, sy, sz, src, dx, dy, dz);
      if (dy != sy) return padImage3d_yz (sx, sy, sz, src,     dy, dz);
      if (dz != sz) return padImage3d__z (sx, sy, sz, src,         dz);
      /*---------*/ return padImage3d___ (sx, sy, sz, src            );
    }

    const float ImageRenderer3d::DEFAULT_BOX_COLOR [] = {0.0, 0.0, 1.0, 0.25};

    ImageRenderer3d::ImageRenderer3d (const Image * img, const Lut * lut) :
      ImageRenderer (),
      m_image_tex3d (0),
      m_steps (),
      m_lut_tex1d (),
      m_lut_tex2d (),
      m_box (),
      m_box_color (),
      m_slice (SLICE_VIEW_CUBE),
      m_textured (true),
      m_outlined (false),
      m_boxed (true)
    {
      m_steps [0] = 1.0;
      m_steps [1] = 8.0;
      copy (DEFAULT_BOX_COLOR, DEFAULT_BOX_COLOR + 4, m_box_color);
      this->setImg (img);
      this->setLut (lut);
    }

    ImageRenderer3d::~ImageRenderer3d ()
    {
    }

    void ImageRenderer3d::setImg (const Image * img)
    {
      //cout << "--> ImageRenderer3d::setImg (this = " << this << ", img = " << img << ");" << endl;
      if (m_image_tex3d != 0)
      {
        glDeleteTextures (1, &m_image_tex3d);
        m_image_tex3d = 0;
        m_image_scales [0] = 1.0;
        m_image_scales [1] = 1.0;
        m_image_scales [2] = 1.0;
      }

      if (img != NULL)
      {
        const unsigned short d1x = img->dx ();
        const unsigned short d1y = img->dy ();
        const unsigned short d1z = img->dz ();

        unsigned short d2x = (1 << (unsigned short) ceil (log2 (d1x)));
        unsigned short d2y = (1 << (unsigned short) ceil (log2 (d1y)));
        unsigned short d2z = (1 << (unsigned short) ceil (log2 (d1z)));

        //cout << d1x << 'x' << d1y << 'x' << d1z << " --> "
             //<< d2x << 'x' << d2y << 'x' << d2z << endl;

        glGenTextures (1, &m_image_tex3d);
        m_image_scales [0] = (double) d1x / d2x;
        m_image_scales [1] = (double) d1y / d2y;
        m_image_scales [2] = (double) d1z / d2z;
        m_box = img->box ();

        glBindTexture (GL_TEXTURE_3D, m_image_tex3d);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#ifdef GL_SGIS_generate_mipmap
        /*if (Extensions::supports ("GL_SGIS_generate_mipmap"))
          {
            cout << "SGIS_generate_mipmap : on\n";
            glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri (GL_TEXTURE_3D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
            //cout << "glTexParameter (GL_GENERATE_MIPMAP_SGIS) : " << gluErrorString (glGetError ()) << endl;
          }
        else*/
#endif
          {
            //cout << "SGIS_generate_mipmap : off\n";
            //GLint interpolation = m_interpolation ? GL_LINEAR : GL_NEAREST;
            glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          }
        //const Lut & lut = m_r8->lut ();
        //glColorTable (GL_TEXTURE_3D, GL_RGBA, 256, GL_RGBA, GL_UNSIGNED_BYTE, lut.data ());
        //cout << "glColorTable : " << gluErrorString (glGetError ()) << endl;
        const unsigned char * b2 = padImage3d (d1x, d1y, d1z, img->begin (), d2x, d2y, d2z);
        glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
#ifdef PRE_CLASSIFICATION
        glTexImage3D (GL_TEXTURE_3D, 0,
                      GL_COLOR_INDEX8_EXT, d2x, d2y, d2z, 0,
                      GL_COLOR_INDEX, GL_UNSIGNED_BYTE, b2);
#else
        glTexImage3D (GL_TEXTURE_3D, 0,
                      GL_LUMINANCE8_EXT, d2x, d2y, d2z, 0,
                      GL_LUMINANCE, GL_UNSIGNED_BYTE, b2);
#endif
        //cout << "glTexImage3D : " << gluErrorString (glGetError ()) << endl;
        delete[] b2;
      }
      //cout << "<-- ImageRenderer3d::setImg (this = " << this << ", img = " << img << ");" << endl;
    }

    GLdouble alpha (GLdouble a, GLdouble d)
    {
      static const GLdouble MAX = 1.0 - exp (-1.0);
      //return 1.0 - pow (1.0 - (a / 255.0), d);
      return (1.0 - exp (-d * (a / 255.0))) / MAX;
      //return d * (a / 255.0);
      //return (a / 255.0);
    }

    GLdouble mix (GLdouble src, GLdouble dst, GLdouble t)
    {
      return t * src + (1.0 - t) * dst;
    }

    void ImageRenderer3d::setLut (const Lut * lut)
    {
      //cout << "--> ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;
      if (m_lut_tex1d [0] != 0)
      {
        glDeleteTextures (1, &(m_lut_tex1d [0])); m_lut_tex1d [0] = 0;
        glDeleteTextures (1, &(m_lut_tex1d [1])); m_lut_tex1d [1] = 0;
        glDeleteTextures (1, &(m_lut_tex2d [0])); m_lut_tex2d [0] = 0;
        glDeleteTextures (1, &(m_lut_tex2d [1])); m_lut_tex2d [1] = 0;
      }

      if (lut != NULL)
      {
        m_lut_tex1d [0] = lut->glTex1d (m_steps [0]);
        m_lut_tex1d [1] = lut->glTex1d (m_steps [1]);
        m_lut_tex2d [0] = lut->glTex2d (m_steps [0]);
        m_lut_tex2d [1] = lut->glTex2d (m_steps [1]);
      }
      else
      {
        static const Lut DEFAULT = Lut ();
        m_lut_tex1d [0] = DEFAULT.glTex1d (m_steps [0]);
        m_lut_tex1d [1] = DEFAULT.glTex1d (m_steps [1]);
        m_lut_tex2d [0] = DEFAULT.glTex2d (m_steps [0]);
        m_lut_tex2d [1] = DEFAULT.glTex2d (m_steps [1]);
      }

      //cout << "<-- ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;
    }

    double ImageRenderer3d::getStep (bool motion) const {return m_steps [motion ? 1 : 0];}
    void ImageRenderer3d::setStep (bool motion, double step) {m_steps [motion ? 1 : 0] = step;}

    enum ImageRenderer3d::Slice ImageRenderer3d::NextSlice (enum ImageRenderer3d::Slice s)
    {
      switch (s)
      {
#if 0
        case SLICE_OBJECT_X   : return SLICE_OBJECT_Y;
        case SLICE_OBJECT_Y   : return SLICE_OBJECT_Z;
        case SLICE_OBJECT_Z   : return SLICE_VIEW_CUBE;
        case SLICE_VIEW_CUBE  : return SLICE_VIEW_TETRA;
        case SLICE_VIEW_TETRA : return SLICE_VIEW_NAIVE;
        case SLICE_VIEW_NAIVE : return SLICE_OBJECT_X;
#else
        case SLICE_VIEW_CUBE  : return SLICE_VIEW_TETRA;
        case SLICE_VIEW_TETRA : return SLICE_VIEW_NAIVE;
        case SLICE_VIEW_NAIVE : return SLICE_VIEW_CUBE;
#endif
        default               : return SLICE_NONE;
      }
    }

    const char * ImageRenderer3d::StrSlice (enum ImageRenderer3d::Slice s)
    {
      switch (s)
      {
        case SLICE_OBJECT_X   : return "SLICE_OBJECT_X";
        case SLICE_OBJECT_Y   : return "SLICE_OBJECT_Y";
        case SLICE_OBJECT_Z   : return "SLICE_OBJECT_Z";
        case SLICE_VIEW_CUBE  : return "SLICE_VIEW_CUBE";
        case SLICE_VIEW_TETRA : return "SLICE_VIEW_TETRA";
        case SLICE_VIEW_NAIVE : return "SLICE_VIEW_NAIVE";
        default               : return "?";
      }
    }

    enum ImageRenderer3d::Slice ImageRenderer3d::getSlice () const {return m_slice;}
    void ImageRenderer3d::setSlice (enum Slice s) {m_slice = s;}

    bool ImageRenderer3d::isOutlined () const {return m_outlined;}
    void ImageRenderer3d::setOutlined (bool b) {m_outlined = b;}

    bool ImageRenderer3d::isTextured () const {return m_textured;}
    void ImageRenderer3d::setTextured (bool b) {m_textured = b;}

    bool ImageRenderer3d::isBoxed () const {return m_boxed;}
    void ImageRenderer3d::setBoxed (bool b) {m_boxed = b;}

    void ImageRenderer3d::glSlice_ObjectAligned_X (bool motion) const
    {
      const vR3 & dim = m_box.dim ();
      double step = m_steps [motion ? 1 : 0] / dim.x;
      for (double x1 = 0.5 * fmod (1.0, step), x2 = 1.0 - x1; x2 >= 0.0; x1 += step, x2 -= step)
      {
        glBegin (GL_QUADS);
        glVertex3d (x1, 0.0, 0.0); glVertex3d (x1, 1.0, 0.0); glVertex3d (x1, 1.0, 1.0); glVertex3d (x1, 0.0, 1.0);
        glVertex3d (x2, 0.0, 0.0); glVertex3d (x2, 0.0, 1.0); glVertex3d (x2, 1.0, 1.0); glVertex3d (x2, 1.0, 0.0);
        glEnd ();
      }
    }

    void ImageRenderer3d::glSlice_ObjectAligned_Y (bool motion) const
    {
      const vR3 & dim = m_box.dim ();
      double step = m_steps [motion ? 1 : 0] / dim.y;
      for (double y1 = 0.5 * fmod (1.0, step), y2 = 1.0 - y1; y2 >= 0.0; y1 += step, y2 -= step)
      {
        glBegin (GL_QUADS);
        glVertex3d (0.0, y2, 0.0); glVertex3d (1.0, y2, 0.0); glVertex3d (1.0, y2, 1.0); glVertex3d (0.0, y2, 1.0);
        glVertex3d (0.0, y1, 0.0); glVertex3d (0.0, y1, 1.0); glVertex3d (1.0, y1, 1.0); glVertex3d (1.0, y1, 0.0);
        glEnd ();
      }
    }

    void ImageRenderer3d::glSlice_ObjectAligned_Z (bool motion) const
    {
      const vR3 & dim = m_box.dim ();
      double step = m_steps [motion ? 1 : 0] / dim.z;
      for (double z1 = 0.5 * fmod (1.0, step), z2 = 1.0 - z1; z2 >= 0.0; z1 += step, z2 -= step)
      {
        glBegin (GL_QUADS);
        glVertex3d (0.0, 0.0, z1); glVertex3d (1.0, 0.0, z1); glVertex3d (1.0, 1.0, z1); glVertex3d (0.0, 1.0, z1);
        glVertex3d (0.0, 0.0, z2); glVertex3d (0.0, 1.0, z2); glVertex3d (1.0, 1.0, z2); glVertex3d (1.0, 0.0, z2);
        glEnd ();
      }
    }

    void ImageRenderer3d::glSlice_ViewAligned_Cube (bool motion) const
    {
      GLdouble m3d [16];
      glGetDoublev (GL_MODELVIEW_MATRIX, m3d);

      // Bounding box depth per vertex.
      GLfloat vZ [8]; // cube depths.
      for (int i = 0; i < 8; ++i)
        vZ [i] = (i & 1 ? m3d [2] : 0) + (i & 2 ? m3d [6] : 0) + (i & 4 ? m3d [10] : 0) + m3d [14];

      // Maximum and minimum depths.
      GLfloat zMin = vZ [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = vZ [i];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Bounding box edges.
      static const int EDGE_ENDS [12][2] = {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      double eZ [12][2];
      for (int i = 0; i < 12; ++i)
      {
        eZ [i][0] = vZ [EDGE_ENDS [i][0]];
        eZ [i][1] = vZ [EDGE_ENDS [i][1]];
      }

      double edgeIntersections [12][3] = {
        {NAN, 0.0, 0.0}, //  0
        {NAN, 0.0, 1.0}, //  1
        {NAN, 1.0, 0.0}, //  2
        {NAN, 1.0, 1.0}, //  3
        {0.0, NAN, 0.0}, //  4
        {0.0, NAN, 1.0}, //  5
        {1.0, NAN, 0.0}, //  6
        {1.0, NAN, 1.0}, //  7
        {0.0, 0.0, NAN}, //  8
        {0.0, 1.0, NAN}, //  9
        {1.0, 0.0, NAN}, // 10
        {1.0, 1.0, NAN}  // 11
      };

      // Back-to-Front.
      GLfloat step = m_steps [motion ? 1 : 0];
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, step); z <= zMax; z += step)
        {
          // This bitfield tells us which vertices of the bounding box
          // have already been passed by the "clipping" plane.
          int tableIndex = 0x00;
          for (int k = 8; k--;) if (vZ [k] < z) tableIndex |= (1 << k);

          const EdgeIndex * t = TABLE [tableIndex];
          if (t[0] != -1)
            {
              glBegin (m_outlined ? GL_LINE_STRIP : GL_TRIANGLE_STRIP);

              // At least 3 vertices.
              EdgeIndex t0 = t[0];
              GLdouble * e0 = edgeIntersections [t0];
              e0 [t0 >> 2] = (eZ [t0][0] - z) / (eZ [t0][0] - eZ [t0][1]);
              glVertex3dv (e0);

              EdgeIndex t1 = t[1];
              GLdouble * e1 = edgeIntersections [t1];
              e1 [t1 >> 2] = (eZ [t1][0] - z) / (eZ [t1][0] - eZ [t1][1]);
              glVertex3dv (e1);

              EdgeIndex t2 = t[2];
              GLdouble * e2 = edgeIntersections [t2];
              e2 [t2 >> 2] = (eZ [t2][0] - z) / (eZ [t2][0] - eZ [t2][1]);
              glVertex3dv (e2);

              // And maybe more...
              for (int k = 3; t[k] != -1;)
                {
                  EdgeIndex tk = t[k++];
                  double * ek = edgeIntersections [tk];
                  ek [tk >> 2] = (eZ [tk][0] - z) / (eZ [tk][0] - eZ [tk][1]);
                  glVertex3dv (ek);
                }

              glEnd ();
            }
        }
    }

    void __glSlice_ViewAligned_Tetra_aux2 (const pR3 & v0, const pR3 & v1, GLfloat z0, GLfloat z1, GLfloat z)
    {
      GLfloat k = (z - z0) / (z1 - z0);
      //pR3 v (v0 + k * (v1 - v0));
      //glVertex3d (v.x, v.y, v.z);
      glVertex3f (v0.x + k * (v1.x - v0.x),
                  v0.y + k * (v1.y - v0.y),
                  v0.z + k * (v1.z - v0.z));
    }

    void __glSlice_ViewAligned_Tetra_aux1 (const pR3 V [8], const GLfloat Z [8], GLfloat z, int i0, int i1, int i2, int i3)
    {
      int flags = 0x00;
      if (Z [i0] < z) flags |= 1;
      if (Z [i1] < z) flags |= 2;
      if (Z [i2] < z) flags |= 4;
      if (Z [i3] < z) flags |= 8;

      glBegin (GL_TRIANGLE_FAN);
      switch (flags)
      {
        case 0x00:
          break;
        case 0x01: // ok !
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i3], Z[i0], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i2], Z[i0], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i1], Z[i0], Z[i1], z);
          break;
        case 0x02:
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i0], Z[i1], Z[i0], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i2], Z[i1], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i3], Z[i1], Z[i3], z);
          break;
        case 0x03: // ok !
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i2], Z[i1], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i3], Z[i1], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i3], Z[i0], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i2], Z[i0], Z[i2], z);
          break;
        case 0x04:
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i3], Z[i2], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i1], Z[i2], Z[i1], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i0], Z[i2], Z[i0], z);
          break;
        case 0x05: // ok ?
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i3], Z[i0], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i3], Z[i2], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i2], Z[i1], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i1], Z[i0], Z[i1], z);
          break;
        case 0x06:
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i2], Z[i0], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i3], Z[i2], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i3], Z[i1], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i1], Z[i0], Z[i1], z);
          break;
        case 0x07: // ok !
          __glSlice_ViewAligned_Tetra_aux2 (V[i3], V[i0], Z[i3], Z[i0], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i3], V[i2], Z[i3], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i3], V[i1], Z[i3], Z[i1], z);
          break;
        case 0x08: // ok !
          __glSlice_ViewAligned_Tetra_aux2 (V[i3], V[i1], Z[i3], Z[i1], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i3], V[i2], Z[i3], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i3], V[i0], Z[i3], Z[i0], z);
          break;
        case 0x09:
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i1], Z[i0], Z[i1], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i3], Z[i1], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i3], Z[i2], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i2], Z[i0], Z[i2], z);
          break;
        case 0x0A: // ok ?
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i1], Z[i0], Z[i1], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i2], Z[i1], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i3], Z[i2], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i3], Z[i0], Z[i3], z);
          break;
        case 0x0B: // ok !
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i0], Z[i2], Z[i0], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i1], Z[i2], Z[i1], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i2], V[i3], Z[i2], Z[i3], z);
          break;
        case 0x0C: // ok !
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i2], Z[i0], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i3], Z[i0], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i3], Z[i1], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i2], Z[i1], Z[i2], z);
          break;
        case 0x0D: // ok !
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i3], Z[i1], Z[i3], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i2], Z[i1], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i1], V[i0], Z[i1], Z[i0], z);
          break;
        case 0x0E:
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i1], Z[i0], Z[i1], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i2], Z[i0], Z[i2], z);
          __glSlice_ViewAligned_Tetra_aux2 (V[i0], V[i3], Z[i0], Z[i3], z);
          break;
        case 0x0F:
          break;
        }
      glEnd ();
    }

    void ImageRenderer3d::glSlice_ViewAligned_Tetra (bool motion) const
    {
      GLdouble m3d [16];
      glGetDoublev (GL_MODELVIEW_MATRIX, m3d);

      // Cube vertices.
      static const pR3 V [8] = {
        pR3 (0.0, 0.0, 0.0),
        pR3 (1.0, 0.0, 0.0),
        pR3 (0.0, 1.0, 0.0),
        pR3 (1.0, 1.0, 0.0),
        pR3 (0.0, 0.0, 1.0),
        pR3 (1.0, 0.0, 1.0),
        pR3 (0.0, 1.0, 1.0),
        pR3 (1.0, 1.0, 1.0)
      };

      // Projected cube vertices.
      GLfloat Z [8];
      for (int i = 0; i < 8; ++i)
        Z [i] = (i & 1 ? m3d [2] : 0) + (i & 2 ? m3d [6] : 0) + (i & 4 ? m3d [10] : 0) + m3d [14];

      // Maximum and minimum depths.
      GLfloat zMin = Z [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = Z [i];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Back-to-Front.
      GLfloat step = m_steps [motion ? 1 : 0];
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, step); z <= zMax; z += step)
      {
        __glSlice_ViewAligned_Tetra_aux1 (V, Z, z, 0, 1, 5, 7);
        __glSlice_ViewAligned_Tetra_aux1 (V, Z, z, 0, 5, 4, 7);
        __glSlice_ViewAligned_Tetra_aux1 (V, Z, z, 0, 4, 6, 7);
        __glSlice_ViewAligned_Tetra_aux1 (V, Z, z, 0, 6, 2, 7);
        __glSlice_ViewAligned_Tetra_aux1 (V, Z, z, 0, 2, 3, 7);
        __glSlice_ViewAligned_Tetra_aux1 (V, Z, z, 0, 3, 1, 7);
      }
    }

    void ImageRenderer3d::glSlice_ViewAligned_Naive (bool motion) const
    {
      GLdouble m3d [16];
      glGetDoublev (GL_MODELVIEW_MATRIX, m3d);

      // Projected cube vertices.
      pR3 projVertices [8];
      for (int i = 0; i < 8; ++i)
        projVertices [i] = pR3 ((i & 1 ? m3d [0] : 0) + (i & 2 ? m3d [4] : 0) + (i & 4 ? m3d [ 8] : 0) + m3d [12],
                                (i & 1 ? m3d [1] : 0) + (i & 2 ? m3d [5] : 0) + (i & 4 ? m3d [ 9] : 0) + m3d [13],
                                (i & 1 ? m3d [2] : 0) + (i & 2 ? m3d [6] : 0) + (i & 4 ? m3d [10] : 0) + m3d [14]);

      // Maximum and minimum depths.
      GLfloat zMin = projVertices [0].z, zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = projVertices [i].z;
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Bounding box edges.
      static const VertexIndex EDGE_VERTICES [12][2] = {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      pR3 projEdgeVertices [12][2];
      for (int i = 0; i < 12; ++i)
      {
        projEdgeVertices [i][0] = projVertices [EDGE_VERTICES [i][0]];
        projEdgeVertices [i][1] = projVertices [EDGE_VERTICES [i][1]];
      }

      GLfloat edgeIntersections [12][3] = {
        {NAN, 0.0, 0.0},  //  0
        {NAN, 0.0, 1.0},  //  1
        {NAN, 1.0, 0.0},  //  2
        {NAN, 1.0, 1.0},  //  3
        {0.0, NAN, 0.0},  //  4
        {0.0, NAN, 1.0},  //  5
        {1.0, NAN, 0.0},  //  6
        {1.0, NAN, 1.0},  //  7
        {0.0, 0.0, NAN},  //  8
        {0.0, 1.0, NAN},  //  9
        {1.0, 0.0, NAN},  // 10
        {1.0, 1.0, NAN}}; // 11

      pR3 projEdgeIntersections [12];

      // Back-to-Front.
      GLfloat step = m_steps [motion ? 1 : 0];
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, step); z <= zMax; z += step)
        {
          vector<EdgeIndex> realIntersections;
          for (int i = 0; i < 12; ++i)
          {
            GLfloat dz = projEdgeVertices [i][0].z - projEdgeVertices [i][1].z;
            if (dz != 0.0)
            {
              GLfloat k = (projEdgeVertices [i][0].z - z) / dz;
              if (k >= 0.0 && k <= 1.0)
              {
                edgeIntersections [i][i >> 2] = k;
                projEdgeIntersections [i] = projEdgeVertices [i][0] + (projEdgeVertices [i][1] - projEdgeVertices [i][0]) * k;
                realIntersections.push_back (i);
              }
            }
          }

          // B.M.E. Moret & H.D. Shapiro "P to NP" pp. 453
          int n = realIntersections.size ();

          pR3 center (0.0, 0.0, 0.0);
          for (int j = 0; j < n; ++j)
          {
            center.x += projEdgeIntersections [j].x;
            center.y += projEdgeIntersections [j].y;
          }
          center.x /= n;
          center.y /= n;

          for (int j = 0; j < n; ++j){
            double theta = -10;
            int next = j;
            for (int k = j; k < n; ++k){
              vR3 d = projEdgeIntersections [realIntersections [k]] - center;
              if ((d.x == 0) && (d.y == 0)) {
                next = k;
                cout << "What the--" << endl;
                break;
              }
              double tt = d.y / (fabs (d.x) + fabs (d.y));
              if (d.x < 0.0) tt = 2.0 - tt;
              else if (d.y < 0.0) tt = 4.0 + tt;
              if (theta <= tt) {
                next = k;
                theta = tt;
              }
            }
            int tmp = realIntersections [j];
            realIntersections [j] = realIntersections [next];
            realIntersections [next] = tmp;
          }

          glBegin (GL_POLYGON);
          for (int i = n; i--;)
            glVertex3fv (edgeIntersections [realIntersections [i]]);
          glEnd ();
        }
    }

    void ImageRenderer3d::glSlice (bool motion) const
    {
      glEnable (GL_TEXTURE_GEN_S);
      glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      GLdouble sTexGenPlane [] = {1.0, 0.0, 0.0, 0.0}; glTexGendv (GL_S, GL_OBJECT_PLANE, sTexGenPlane);

      glEnable (GL_TEXTURE_GEN_T);
      glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      GLdouble tTexGenPlane [] = {0.0, 1.0, 0.0, 0.0}; glTexGendv (GL_T, GL_OBJECT_PLANE, tTexGenPlane);

      glEnable (GL_TEXTURE_GEN_R);
      glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      GLdouble rTexGenPlane [] = {0.0, 0.0, 1.0, 0.0}; glTexGendv (GL_R, GL_OBJECT_PLANE, rTexGenPlane);

      switch (m_slice)
      {
        case SLICE_NONE       : break;
        case SLICE_OBJECT_X   : glSlice_ObjectAligned_X   (motion); break;
        case SLICE_OBJECT_Y   : glSlice_ObjectAligned_Y   (motion); break;
        case SLICE_OBJECT_Z   : glSlice_ObjectAligned_Z   (motion); break;
        case SLICE_VIEW_CUBE  : glSlice_ViewAligned_Cube  (motion); break;
        case SLICE_VIEW_TETRA : glSlice_ViewAligned_Tetra (motion); break;
        case SLICE_VIEW_NAIVE : glSlice_ViewAligned_Naive (motion); break;
      }

      glDisable (GL_TEXTURE_GEN_S);
      glDisable (GL_TEXTURE_GEN_T);
      glDisable (GL_TEXTURE_GEN_R);
    }

#if 0
    void ImageRenderer3d::glDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glPushMatrix ();
      GL::Translate (m_box.pos ());
      GL::Scale     (m_box.dim ());
      //cout << "pos = " << pos << ", dim = " << dim << endl;

      GLuint displayList = glGenLists (1);
      glNewList (displayList, GL_COMPILE);
      this->glSlice (motion);
      glEndList ();

      if (m_textured)
      {
        // Lighting OFF.
        glDisable (GL_LIGHTING);
        // Blending ON.
        glEnable (GL_BLEND);
        glDepthMask (GL_FALSE); //FIXME
#ifdef MODE_MIP
        // MIP blending equation.
        glBlendEquation (GL_MIN); // GL_MAX
#else
        // Alpha test ON.
        //glEnable (GL_ALPHA_TEST);
        //glAlphaFunc (GL_GREATER, 0.0); //1.0 - pow (0.7, step));
        glBlendEquation (GL_FUNC_ADD);
        glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif

#ifdef MODE_MIP
        // Bounding box faces.
        static const unsigned char bbf [] = {
          0, 1, 3, 2,  // Far
          0, 2, 6, 4,  // Left
          4, 6, 7, 5,  // Near
          1, 5, 7, 3,  // Right
          0, 4, 5, 1,  // Down
          2, 3, 7, 6}; // Up

        // Bounding box background (BBB).
        glColor4f (1.0, 1.0, 1.0, 0.0); // FIXME: min
        //glColor4f (0.0, 0.0, 0.0, 0.0); // FIXME: max
        glEnableClientState (GL_VERTEX_ARRAY);
        glVertexPointer (3, GL_DOUBLE, 0, bbv);
        glDrawElements (GL_QUADS, 24, GL_UNSIGNED_BYTE, bbf);
        glDisableClientState (GL_VERTEX_ARRAY);
#endif

#ifdef MODE_MIP
        const Lut & lut = m_r8->lut ();
        const unsigned char * l1 = lut.data ();
        unsigned char newLut [1024], * l2 = newLut;
        for (unsigned short k = 256; k--;)
          {
            l2 = copy (l1, l1 + 3, l2); l1 += 4;
            *(l2++) = 255;
          }
        glColorTable (GL_TEXTURE_3D, GL_RGB, 256, GL_RGB, GL_UNSIGNED_BYTE, newLut);
#else
#ifdef PRE_CLASSIFICATION
        const Lut & lut = m_r8->lut ();
        const unsigned char * l1 = lut.data ();
        unsigned char newLut [1024], * l2 = newLut;
        for (unsigned short k = 256; k--; l1 += 4)
        {
          float alpha = (1.0 - pow (1.0 - ((float) l1 [3] / 255), step));
          //cout << (unsigned int) l1 [3] << " --> " << alpha << endl;
          *(l2++) = (unsigned char) rint (alpha * l1 [0]);
          *(l2++) = (unsigned char) rint (alpha * l1 [1]);
          *(l2++) = (unsigned char) rint (alpha * l1 [2]);
          *(l2++) = (unsigned char) rint (alpha * 255);
        }
        glColorTable (GL_TEXTURE_3D, GL_RGBA, 256, GL_RGBA, GL_UNSIGNED_BYTE, newLut);
        //cout << "glColorTable : " << gluErrorString (glGetError ()) << endl;
#else
#ifdef POST_CLASSIFICATION
        glActiveTexture (GL_TEXTURE1);
        glBindTexture (GL_TEXTURE_1D, m_lutTextures [motion ? 1 : 0]);
        glEnable (GL_TEXTURE_SHADER_NV);
        glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexEnvi (GL_TEXTURE_SHADER_NV, GL_PREVIOUS_TEXTURE_INPUT_NV, GL_TEXTURE0);
        glTexEnvi (GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_DEPENDENT_AR_TEXTURE_2D_NV);
#endif
#endif
#endif

        // Texture3d ON.
#ifdef POST_CLASSIFICATION
        glEnable (GL_TEXTURE_SHADER_NV);
        glActiveTexture (GL_TEXTURE0);
#else
        glEnable (GL_TEXTURE_3D);
#endif

        glBindTexture (GL_TEXTURE_3D, m_image_tex3d);
        glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        //cout << "glBindTexture : " << gluErrorString (glGetError ()) << endl;

#ifdef POST_CLASSIFICATION
        glTexEnvi (GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_TEXTURE_3D);
#endif

        // We use the texture matrix to deal with the 2**k padding.
        glMatrixMode (GL_TEXTURE);
        glPushMatrix ();
        glLoadIdentity ();
        glScaled (m_image_scales [0], m_image_scales [1], m_image_scales [2]);
        glMatrixMode (GL_MODELVIEW);

        if (m_outlined)
        {
          glEnable (GL_POLYGON_OFFSET_FILL);
          glPolygonOffset (1.0, 1.0);
          glCallList (displayList);
          glDisable (GL_POLYGON_OFFSET_FILL);
        }
        else
          glCallList (displayList);

        glDisable (GL_TEXTURE_3D);
        glDisable (GL_BLEND);
        glDepthMask (GL_TRUE);
#ifdef MODE_MIP
        glBlendEquation (GL_FUNC_ADD);
#else
        if (GLEW_EXT_blend_func_separate)
          glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                               GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);
        else
          glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
#ifdef POST_CLASSIFICATION
        glActiveTexture (GL_TEXTURE0);
        glDisable (GL_TEXTURE_SHADER_NV);
#endif
        glMatrixMode (GL_TEXTURE);
        glPopMatrix ();
        glMatrixMode (GL_MODELVIEW);
      }

      if (m_outlined)
      {
        // Draw lines.
        GLint backup_glPolygonMode [2];
        glGetIntegerv (GL_POLYGON_MODE, backup_glPolygonMode);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        // Bold lines.
        GLfloat backup_glLineWidth;
        glGetFloatv (GL_LINE_WIDTH, &backup_glLineWidth);
        glLineWidth (4.0);
        // Bold black lines.
        glColor4f (0.0, 0.0, 0.0, 1.0);
        // Feed with geometry.
        glCallList (displayList);
        // Restore previous state.
        glPolygonMode (GL_FRONT, backup_glPolygonMode [0]);
        glPolygonMode (GL_BACK,  backup_glPolygonMode [1]);
        glLineWidth (backup_glLineWidth);
      }

      glDeleteLists (displayList, 1);
      glPopMatrix ();

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;

      if (m_boxed)
      {
        // Bounding box.
        GLfloat backup_glLineWidth;
        glGetFloatv (GL_LINE_WIDTH, &backup_glLineWidth);
        glLineWidth (4.0);
        Lighting::color (m_box_color);
        Primitives::box (m_box);
        glLineWidth (backup_glLineWidth);
      }
    }
#endif

    void ImageRenderer3d::glDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glPushMatrix ();
      GL::Translate (m_box.pos ());
      GL::Scale     (m_box.dim ());
      //cout << "pos = " << pos << ", dim = " << dim << endl;

      // Store geometry in a display list.
      GLuint displayList = glGenLists (1);
      glNewList (displayList, GL_COMPILE);
      this->glSlice (motion);
      glEndList ();

      if (m_textured)
      {
        // Lighting OFF.
        glDisable (GL_LIGHTING);
        // Blending ON.
        glEnable (GL_BLEND);
        glDepthMask (GL_FALSE); //FIXME
        // Alpha test ON.
        //glBlendEquation (GL_FUNC_ADD);
        if (GLEW_EXT_blend_func_separate)
          //glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
          glBlendFuncSeparate (GL_ONE,       GL_ONE_MINUS_SRC_ALPHA,
                               GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);
        else
          //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glBlendFunc (GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);

        // Bind 1st texture (image).
        glActiveTexture (GL_TEXTURE0);
        glBindTexture (GL_TEXTURE_3D, m_image_tex3d);
        glMatrixMode (GL_TEXTURE);
        glPushMatrix ();
        glLoadIdentity ();
        glScaled (m_image_scales [0], m_image_scales [1], m_image_scales [2]);
        glMatrixMode (GL_MODELVIEW);

        // Bind 2nd texture (lut1d).
        glActiveTexture (GL_TEXTURE1);
        glBindTexture (GL_TEXTURE_1D, m_lut_tex1d [motion ? 1 : 0]);

        // Bind 3rd texture (lut2d).
        glActiveTexture (GL_TEXTURE2);
        glBindTexture (GL_TEXTURE_2D, m_lut_tex2d [motion ? 1 : 0]);

        // Activate texture unit #0.
        glActiveTexture (GL_TEXTURE0);

        this->glPreDraw (motion);

        if (m_outlined)
        {
          glEnable (GL_POLYGON_OFFSET_FILL);
          glPolygonOffset (1.0, 1.0);
          glCallList (displayList);
          //this->glSlice (motion);
          glDisable (GL_POLYGON_OFFSET_FILL);
        }
        else
          glCallList (displayList);
          //this->glSlice (motion);

        this->glPostDraw (motion);

        glDisable (GL_BLEND);
        glDepthMask (GL_TRUE);
        if (GLEW_EXT_blend_func_separate)
          glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                               GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);
        else
          glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glMatrixMode (GL_TEXTURE);
        glPopMatrix ();
        glMatrixMode (GL_MODELVIEW);
      }

      if (m_outlined)
      {
        // Draw lines.
        GLint backup_glPolygonMode [2];
        glGetIntegerv (GL_POLYGON_MODE, backup_glPolygonMode);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        // Bold lines.
        GLfloat backup_glLineWidth;
        glGetFloatv (GL_LINE_WIDTH, &backup_glLineWidth);
        glLineWidth (4.0);
        // Bold black lines.
        glColor4f (0.0, 0.0, 0.0, 1.0);
        // Feed with geometry.
        glCallList (displayList);
        //this->glSlice (motion);
        // Restore previous state.
        glPolygonMode (GL_FRONT, backup_glPolygonMode [0]);
        glPolygonMode (GL_BACK,  backup_glPolygonMode [1]);
        glLineWidth (backup_glLineWidth);
      }

      glDeleteLists (displayList, 1);
      glPopMatrix ();

      if (m_boxed)
      {
        // Bounding box.
        GLfloat backup_glLineWidth;
        glGetFloatv (GL_LINE_WIDTH, &backup_glLineWidth);
        glLineWidth (4.0);
        GL::Lighting::Color (m_box_color);
        GL::Primitives::Box (m_box);
        glLineWidth (backup_glLineWidth);
      }

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }
  } // namespace R3d
} // namespace Aa

