#ifndef AA_R3D_LUT__H
#define AA_R3D_LUT__H

#include <GL/glew.h>
#include <AaVector>
#include "AaR3d.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API Lut
    {
      public:
        typedef V<GLubyte, 4> Entry;

      private:
        Entry m_data [256];

      private:
        static GLubyte  Round (GLdouble);
        static GLdouble Alpha (GLdouble a, GLdouble d);

      public:
        Lut ();
        Lut (const Entry   [256]);
        Lut (const GLubyte [256][4]);
        Lut (const std::string &) throw (Aa::FileNotFound, Aa::ParseError);
        Lut (const Lut &);
        // default dtor.
        Lut & operator= (const Lut &);
        const Entry * data () const;
        const Entry & operator[] (int k) const;
        GLuint glTex1d (GLdouble step) const;
        GLuint glTex2d (GLdouble step) const;
        void load (const std::string &) throw (Aa::FileNotFound, Aa::ParseError);
    };
  } // namespace R3d
} // namespace Aa

#endif // AA_R3D_LUT__H

