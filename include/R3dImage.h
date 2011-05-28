#ifndef __AA_R3D_IMAGE__
#define __AA_R3D_IMAGE__

#include <string>
#include <AaMath>
#include <AaException>

// Image.

namespace Aa
{
  namespace R3d
  {
    class Image
    {
     protected:
      unsigned short m_dx, m_dy, m_dz;
      unsigned long m_dxdy, m_dxdydz;
      unsigned char * m_data;
      Math::Box m_box;

     public:
      // Constructor.
      Image (unsigned short dx = 1, unsigned short dy = 1, unsigned short dz = 1);
      // Destructor.
      virtual ~Image ();
      // Reallocation.
      void resize (unsigned short dx = 1, unsigned short dy = 1, unsigned short dz = 1);
      // Dimensions.
      unsigned short dx () const;
      unsigned short dy () const;
      unsigned short dz () const;
      // Slice size.
      unsigned long dxdy  () const;
      // Stack size.
      unsigned long dxdydz () const;
      // Bounding box.
      void setBox (const Math::Box &);
      const Math::Box & box () const;
      //const Math::pR3 & getBoxPos () const;
      //const Math::vR3 & getBoxDim () const;
      void translate (const Math::vR3 &);
      // Iterators.
      unsigned char       * begin ();
      const unsigned char * begin () const;
      unsigned char       * end ();
      const unsigned char * end () const;
      // Potential.
      double triLinear (const Math::pR3 &) const;
      double triLinear (double, double, double) const;
    };

    // Loading.
    void ImageLoadB8 (Image *, const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    Image * ImageLoadB8 (const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    void ImageLoadPIC (Image *, const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    Image * ImageLoadPIC (const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    void ImageLoadDICOM (Image *, const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

    Image * ImageLoadDICOM (const std::string &)
      throw (Aa::FileNotFound, Aa::ParseError);

  } // namespace R3d
} // namespace Aa

#endif // __AA_R3D_IMAGE__

