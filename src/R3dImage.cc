#include <fstream>
#include <cmath>
#include "R3dImage.h"

using namespace std;

#ifdef DEBUG
#define ASSERT(x) assert (x)
#else
#define ASSERT(x)
#endif

namespace Aa
{
  namespace R3d
  {

    inline double linear (double du,
                            double u0, double u1)
    {
      return u0 + du * (u1 - u0);
    }

    inline double biLinear (double du, double dv,
                            double u0, double u1, double v0, double v1)
    {
      return linear (dv,
                     linear (du, u0, u1),
                     linear (du, v0, v1));
    }

    inline double triLinear (double du, double dv, double dw,
                             double d0, double d1, double d2, double d3,
                             double d4, double d5, double d6, double d7)
    {
      return linear (dw,
                     biLinear (du, dv, d0, d1, d2, d3),
                     biLinear (du, dv, d4, d5, d6, d7));
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::Image //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Image::Image (unsigned short dx, unsigned short dy, unsigned short dz) :
      m_dx (1), m_dy (1), m_dz (1),
      m_dxdy (1), m_dxdydz (1),
      m_data (new unsigned char [1])
    {
      this->resize (dx, dy, dz);
    }

    Image::~Image ()
    {
      delete[] m_data;
    }

    void Image::resize (unsigned short dx, unsigned short dy, unsigned short dz)
    {
      //cout << "--> Image::resize (" << dx << ", "<< dy << ", " << dz << ");" << endl;
      delete[] m_data;
      m_dx = max ((unsigned short) 1, dx);
      m_dy = max ((unsigned short) 1, dy);
      m_dz = max ((unsigned short) 1, dz);
      m_dxdy = m_dx * m_dy;
      m_dxdydz = m_dxdy * m_dz;
      m_box = Math::Box (0.0, 0.0, 0.0, m_dx, m_dy, m_dz);
      m_data = new unsigned char [m_dxdydz];
      //cout << "<-- Image::resize (" << dx << ", "<< dy << ", " << dz << ");" << endl;
    }

    unsigned short Image::dx () const {return m_dx;}
    unsigned short Image::dy () const {return m_dy;}
    unsigned short Image::dz () const {return m_dz;}

    unsigned long Image::dxdy  () const {return m_dxdy;}
    unsigned long Image::dxdydz () const {return m_dxdydz;}

    void Image::setBox (const Math::Box & b) {m_box = b;}
    const Math::Box & Image::box () const {return m_box;}
    //const Math::pR3 & Image::getBoxPos () const {return _box.getPos ();}
    //const Math::vR3 & Image::getBoxDim () const {return _box.getDim ();}
    void Image::translate (const Math::vR3 & v) {m_box.translate (v);}

    unsigned char * Image::begin () {return m_data;}
    unsigned char * Image::end   () {return m_data + m_dxdydz;}

    const unsigned char * Image::begin () const {return m_data;}
    const unsigned char * Image::end   () const {return m_data + m_dxdydz;}

    double Image::triLinear (const Math::pR3 & p) const
    {
      return this->triLinear (p.x, p.y, p.z);
    }

    double Image::triLinear (double px, double py, double pz) const
    {
      // Spéciale dédicace à Emmanuel!
      //if (Math::vR3 (-1.0, 1.8, 0.0) % Math::vR3 (px - 0.5, py - 0.0, pz - 0.0) < 0.0) return 0.0;
      //if (Math::vR3 (+1.0, 3.0, 0.0) % Math::vR3 (px - 0.5, py - 1.0, pz - 0.0) > 0.0) return 0.0;

      double x = (px * m_dx) - 0.5, u = floor (x); long int_u = (long) u;
      double y = (py * m_dy) - 0.5, v = floor (y); long int_v = (long) v;
      double z = (pz * m_dz) - 0.5, w = floor (z); long int_w = (long) w;

      // 8 bits for 8 corners.
      unsigned char f = 0xFF;
      if (int_u + 1 <     0) f = 0x00; else if (int_u     <     0) f &= (0x02 | 0x08 | 0x20 | 0x80);
      if (int_u     >= m_dx) f = 0x00; else if (int_u + 1 >= m_dx) f &= (0x01 | 0x04 | 0x10 | 0x40);
      if (int_v + 1 <     0) f = 0x00; else if (int_v     <     0) f &= (0x04 | 0x08 | 0x40 | 0x80);
      if (int_v     >= m_dy) f = 0x00; else if (int_v + 1 >= m_dy) f &= (0x01 | 0x02 | 0x10 | 0x20);
      if (int_w + 1 <     0) f = 0x00; else if (int_w     <     0) f &= (0x10 | 0x20 | 0x40 | 0x80);
      if (int_w     >= m_dz) f = 0x00; else if (int_w + 1 >= m_dz) f &= (0x01 | 0x02 | 0x04 | 0x08);

      const unsigned char * d = m_data + ((int_w * m_dy + int_v) * m_dx + int_u); 
      return Aa::R3d::triLinear (x - u, y - v, z - w,
                                 (f & 0x01) ? *(d                    ) : 0,
                                 (f & 0x02) ? *(d                 + 1) : 0,
                                 (f & 0x04) ? *(d          + m_dx    ) : 0,
                                 (f & 0x08) ? *(d          + m_dx + 1) : 0,
                                 (f & 0x10) ? *(d + m_dxdy           ) : 0,
                                 (f & 0x20) ? *(d + m_dxdy        + 1) : 0,
                                 (f & 0x40) ? *(d + m_dxdy + m_dx    ) : 0,
                                 (f & 0x80) ? *(d + m_dxdy + m_dx + 1) : 0);
    }

    void ImageLoadB8 (Image * image, const string & filename)
      throw (Aa::FileNotFound, Aa::ParseError)
    {
      cout << "ImageLoadB8 (\"" << filename << "\");" << endl;
      ifstream f (filename.c_str (), ios::in | ios::binary);
      if (! f) throw FileNotFound (filename);
      // Read dimensions.
      unsigned short dx = 0;
      if (! f.read ((char *) &dx, sizeof (dx))) {f.close (); throw Aa::ParseError ("dx");}
      unsigned short dy = 0;
      if (! f.read ((char *) &dy, sizeof (dy))) {f.close (); throw Aa::ParseError ("dy");}
      unsigned short dz = 0;
      if (! f.read ((char *) &dz, sizeof (dz))) {f.close (); throw Aa::ParseError ("dz");}
      // Resize buffer.
      image->resize (dx, dy, dz);
      // Read data.
      if (! f.read ((char *) image->begin (), image->dxdydz ())) {f.close (); throw Aa::ParseError ("dat");}
      f.close ();
    }

    Image * ImageLoadB8 (const string & filename)
      throw (Aa::FileNotFound, Aa::ParseError)
    {
      Image * image = new Image ();
      try {ImageLoadB8 (image, filename);} catch (...) {delete image; throw;}
      return image;
    }
  } // namespace R3d
} // namespace Aa

