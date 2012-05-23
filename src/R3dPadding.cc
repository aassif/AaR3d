#include <algorithm>
#include "R3dPadding.h"

namespace Aa
{
  namespace R3d
  {

    unsigned char * pad3dxyz (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src,
                              unsigned short dx, unsigned short dy, unsigned short dz)
    {
#if 0
      cout << "pad3dxyz ("
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
      std::fill (dst, dst + dxdydz, 0);
      unsigned long dxvy = dx * vy;
      for (unsigned short z = sz; z--; d += dxvy)
        for (unsigned short y = sy; y--; d += dx) std::copy (s, s += sx, d);
#else
      // The 2 loops (copy border).
      unsigned short vx = dx - sx;
      unsigned short vy = dy - sy;
      unsigned short vz = dz - sz;
      for (unsigned short z = sz; z--;)
      {
        for (unsigned short y = sy; y--;)
        {
          d = std::copy (s, s + sx, d); s += sx;
          unsigned char c = d [-1]; // crash if sx < 1
          std::fill (d, d + vx, c); d += vx;
        }
        for (unsigned short y = vy; y--;)
          d = std::copy (d - dx, d, d); // crash if sy < 1
      }
      for (unsigned short z = vz; z--;)
        d = std::copy (d - dxdy, d, d); // crash if sz < 1
#endif
      return dst;
    }

    unsigned char * pad3d_yz (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src,
                              unsigned short dy, unsigned short dz)
    {
#if 0
      cout << "pad3d_yz ("
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
      std::fill (dst, dst + sxdydz, 0);
      for (unsigned short z = sz; z--; d += sxdy) std::copy (s, s += sxsy, d);
#else
      unsigned short vy = dy - sy;
      unsigned short vz = dz - sz;
      // The loop (copy border).
      for (unsigned short z = sz; z--;)
      {
        d = std::copy (s, s + sxsy, d); s += sxsy;
        for (unsigned short y = vy; y--;)
          d = std::copy (d - sx, d, d); // crash if sy < 1
      }
      for (unsigned short z = vz; z--;)
        d = std::copy (d - sxdy, d, d); // crash if sz < 1
#endif
      return dst;
    }

    unsigned char * pad3d__z (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src,
                              unsigned short dz)
    {
#if 0
      cout << "pad3d__z ("
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
      std::fill (dst, dst + sxsydz, 0);
      std::copy (src, src + sxsy * sz, dst);
#else
      // Simple copy.
      unsigned char * d = dst;
      unsigned short vz = dz - sz;
      d = std::copy (src, src + sxsy * sz, d);
      for (unsigned short z = vz; z--;)
        d = std::copy (d - sxsy, d, d); // crash if sz < 1
#endif
      return dst;
    }

    unsigned char * pad3d___ (unsigned short sx, unsigned short sy, unsigned short sz,
                              const unsigned char * src)
    {
#if 0
      cout << "pad3d___ ("
           << "sx = " << sx << ", sy = " << sy << ", sz = " << sz << ", "
           << "src = " << (void *) src << ")\n";
#endif
      unsigned long sxsysz = sx * sy * sz;
      unsigned char * dst = new unsigned char [sxsysz];
      std::copy (src, src + sxsysz, dst);
      return dst;
    }

    unsigned char * pad3d (unsigned short sx, unsigned short sy, unsigned short sz,
                           const unsigned char * src,
                           unsigned short dx, unsigned short dy, unsigned short dz)
    {
      if (dx != sx) return pad3dxyz (sx, sy, sz, src, dx, dy, dz);
      if (dy != sy) return pad3d_yz (sx, sy, sz, src,     dy, dz);
      if (dz != sz) return pad3d__z (sx, sy, sz, src,         dz);
      /*---------*/ return pad3d___ (sx, sy, sz, src            );
    }


  } // namespace R3d
} // namespace Aa

