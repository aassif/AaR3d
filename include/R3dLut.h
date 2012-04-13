#ifndef __AA_R3D_LUT__
#define __AA_R3D_LUT__

#include <GL/glew.h>
#include <AaException>
#include "AaR3d.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Lut
    {
      private:
        GLubyte m_data [256][4];

      private:
        static GLubyte  Round (GLdouble);
        static GLdouble Alpha (GLdouble a, GLdouble d);

      public:
        Lut ();
        Lut (const GLubyte [256][4]);
        Lut (const std::string &) throw (Aa::FileNotFound, Aa::ParseError);
        Lut (const Lut &);
        // default dtor.
        Lut & operator= (const Lut &);
        const GLubyte * data () const;
        const GLubyte * operator[] (int k) const;
        GLuint glTex1d (GLdouble step) const;
        GLuint glTex2d (GLdouble step) const;
        void load (const std::string &) throw (Aa::FileNotFound, Aa::ParseError);
    };
  } // namespace R3d
} // namespace Aa

#endif // __AA_R3D_LUT__

