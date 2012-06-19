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

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::Image //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Image::Image (const uvec3 & d) :
      //m_dxdy (1), m_dxdydz (1),
      //m_data (new unsigned char [1])
      m_image (vec (0u, 0u, 0u)),
      m_box ()
    {
      this->resize (d);
    }

    Image::~Image ()
    {
      //delete[] m_data;
    }

    void Image::resize (const uvec3 & d)
    {
      //cout << "--> Image::resize (" << dx << ", "<< dy << ", " << dz << ");" << endl;
      m_image = Im<3, Mono8> (d);
      //m_dxdy = m_dx * m_dy;
      //m_dxdydz = m_dxdy * m_dz;
      m_box = dbox3::Center (dvec3 (d));
      //m_data = new unsigned char [m_dxdydz];
      //cout << "<-- Image::resize (" << dx << ", "<< dy << ", " << dz << ");" << endl;
    }

    unsigned int Image::dx () const {return m_image.dim (0);}
    unsigned int Image::dy () const {return m_image.dim (1);}
    unsigned int Image::dz () const {return m_image.dim (2);}

    //unsigned long Image::dxdy  () const {return m_dxdy;}
    //unsigned long Image::dxdydz () const {return m_dxdydz;}

    void Image::setBox (const dbox3 & b) {m_box = b;}
    const dbox3 & Image::box () const {return m_box;}
    //void Image::translate (const Math::vR3 & v) {m_box.translate (v);}

    Mono8::Pixel * Image::begin () {return m_image.begin ();}
    Mono8::Pixel * Image::end   () {return m_image.end   ();}

    const Mono8::Pixel * Image::begin () const {return m_image.begin ();}
    const Mono8::Pixel * Image::end   () const {return m_image.end   ();}

    double Image::eval (const dvec3 & p) const
    {
      int dx = this->dx ();
      int dy = this->dy ();
      int dz = this->dz ();

      double x = (p[0] * dx) - 0.5, u = floor (x); int int_u = (int) u;
      double y = (p[1] * dy) - 0.5, v = floor (y); int int_v = (int) v;
      double z = (p[2] * dz) - 0.5, w = floor (z); int int_w = (int) w;

      // 8 bits for 8 corners.
      unsigned char f = 0xFF;
      if (int_u + 1 <   0) f = 0x00; else if (int_u     <   0) f &= (0x02 | 0x08 | 0x20 | 0x80);
      if (int_u     >= dx) f = 0x00; else if (int_u + 1 >= dx) f &= (0x01 | 0x04 | 0x10 | 0x40);
      if (int_v + 1 <   0) f = 0x00; else if (int_v     <   0) f &= (0x04 | 0x08 | 0x40 | 0x80);
      if (int_v     >= dy) f = 0x00; else if (int_v + 1 >= dy) f &= (0x01 | 0x02 | 0x10 | 0x20);
      if (int_w + 1 <   0) f = 0x00; else if (int_w     <   0) f &= (0x10 | 0x20 | 0x40 | 0x80);
      if (int_w     >= dz) f = 0x00; else if (int_w + 1 >= dz) f &= (0x01 | 0x02 | 0x04 | 0x08);

      const Mono8::Pixel * d = m_image.begin () + ((int_w * dy + int_v) * dx + int_u); 
      return TriLinear (x - u, y - v, z - w,
                        (f & 0x01) ? d[             0][0] : 0,
                        (f & 0x02) ? d[           + 1][0] : 0,
                        (f & 0x04) ? d[      + dx    ][0] : 0,
                        (f & 0x08) ? d[      + dx + 1][0] : 0,
                        (f & 0x10) ? d[dx*dy         ][0] : 0,
                        (f & 0x20) ? d[dx*dy      + 1][0] : 0,
                        (f & 0x40) ? d[dx*dy + dx    ][0] : 0,
                        (f & 0x80) ? d[dx*dy + dx + 1][0] : 0);
    }

    void ImageLoadB8 (Image * image, const string & filename)
      throw (Aa::FileNotFound, Aa::ParseError)
    {
      cout << "ImageLoadB8 (\"" << filename << "\");" << endl;
      ifstream f (filename.c_str (), ios::in | ios::binary);
      if (! f.is_open ()) throw FileNotFound (filename);
      // Read dimensions.
      unsigned short dx = 0;
      if (! f.read ((char *) &dx, sizeof (dx))) throw Aa::ParseError ("dx");
      unsigned short dy = 0;
      if (! f.read ((char *) &dy, sizeof (dy))) throw Aa::ParseError ("dy");
      unsigned short dz = 0;
      if (! f.read ((char *) &dz, sizeof (dz))) throw Aa::ParseError ("dz");
      // Resize buffer.
      image->resize (vec<uint> (dx, dy, dz));
      // Read data.
      if (! f.read ((char *) image->begin (), dx * dy * dz)) throw Aa::ParseError ("dat");
    }

    Image * ImageLoadB8 (const string & filename)
      throw (Aa::FileNotFound, Aa::ParseError)
    {
      Image * image = new Image (uvec3 (0u));
      try {ImageLoadB8 (image, filename);} catch (...) {delete image; throw;}
      return image;
    }
  } // namespace R3d
} // namespace Aa

