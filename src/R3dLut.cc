#include <cmath>
#include <fstream>
#include <AaText>
#include "R3dLut.h"

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
// Aa::R3d::Lut ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Lut::Lut () :
      m_data ()
    {
      for (int i = 0; i < 256; ++i)
        m_data [i] = vec<AaUInt8> (255, 255, 255, i);
    }

    Lut::Lut (const Entry data [256]) :
      m_data ()
    {
      copy (data, data + 256, m_data);
    }

    Lut::Lut (const GLubyte data [256][4]) :
      m_data ()
    {
      const Entry * d = (const Entry *) data;
      copy (d, d + 256, m_data);
    }

    Lut::Lut (const string & filename)
      throw (FileNotFound, ParseError) :
      m_data ()
    {
      this->load (filename);
    }

    Lut::Lut (const Lut & lut) :
      m_data ()
    {
      (*this) = lut;
    }

    Lut & Lut::operator= (const Lut & lut)
    {
      const Entry * d = lut.data ();
      copy (d, d + 256, m_data);
      return (*this);
    }

    const Lut::Entry * Lut::data () const
    {
      return m_data;
    }

    const Lut::Entry & Lut::operator[] (int k) const
    {
      return m_data [k];
    }

    GLubyte Lut::Round (GLdouble x)
    {
      return x <= 0.0 ? 0 : (x < 255.0 ? (GLubyte) rint (x) : 255);
    }

    GLdouble Lut::Alpha (GLdouble a, GLdouble d)
    {
      return 1.0 - exp (-d * (a / 255.0));
    }

    GLuint Lut::glTex1d (double step) const
    {
      GLubyte P [256][4];
      GLdouble max = 1.0; //Alpha (255, step);
      for (int i = 0; i < 256; ++i)
      {
        GLdouble d = step * m_data[i][3] / 255.0;
        P[i][0] = Round (d * m_data[i][0]);
        P[i][1] = Round (d * m_data[i][1]);
        P[i][2] = Round (d * m_data[i][2]);
        P[i][3] = Round (255.0 * Alpha (255, d) / max);
      }

      GLuint texture;
      glGenTextures (1, &texture);
      glBindTexture (GL_TEXTURE_1D, texture);
      glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glTexImage1D (GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, P);

      return texture;
    }

    GLuint Lut::glTex2d (double step) const
    {
      GLdouble I [256][4];
      {
        I[0][0] = 0.0;
        I[0][1] = 0.0;
        I[0][2] = 0.0;
        I[0][3] = 0.0;
      }
      for (int i = 1; i < 256; ++i)
      {
        GLdouble a = 0.5 * (m_data[i-1][3] + m_data[i][3]);
        I[i][0] = I[i-1][0] + 0.5 * (m_data[i-1][0] + m_data[i][0]) * (a / 255.0);
        I[i][1] = I[i-1][1] + 0.5 * (m_data[i-1][1] + m_data[i][1]) * (a / 255.0);
        I[i][2] = I[i-1][2] + 0.5 * (m_data[i-1][2] + m_data[i][2]) * (a / 255.0);
        I[i][3] = I[i-1][3] + a;
      }

      GLubyte P [256][256][4];
      GLdouble max = 1.0; //Alpha (255, step);
      for (int i = 0; i < 256; ++i)
      {
        // j < i
        for (int j = 0; j < i; ++j)
        {
          GLdouble d = step / (i - j);
          P[i][j][0] = Round (d * (I[i][0] - I[j][0]));
          P[i][j][1] = Round (d * (I[i][1] - I[j][1]));
          P[i][j][2] = Round (d * (I[i][2] - I[j][2]));
          P[i][j][3] = Round (255.0 * Alpha (I[i][3] - I[j][3], d) / max);
        }

        // j = i
        {
          GLdouble d = step * m_data[i][3] / 255.0;
          P[i][i][0] = Round (d * m_data[i][0]);
          P[i][i][1] = Round (d * m_data[i][1]);
          P[i][i][2] = Round (d * m_data[i][2]);
          P[i][i][3] = Round (255.0 * Alpha (255, d) / max);
        }

        // j > i
        for (int j = i+1; j < 256; ++j)
        {
          GLdouble d = step / (j - i);
          P[i][j][0] = Round (d * (I[j][0] - I[i][0]));
          P[i][j][1] = Round (d * (I[j][1] - I[i][1]));
          P[i][j][2] = Round (d * (I[j][2] - I[i][2]));
          P[i][j][3] = Round (255.0 * Alpha (I[j][3] - I[i][3], d) / max);
        }
      }

#if 0
      {
        ofstream ofs ("pre.rgb.ppm", ios::out | ios::binary);
        ofs << "P6\n256 256\n255\n";
        for (int i = 0; i < 256; ++i)
          for (int j = 0; j < 256; ++j)
          {
            ofs.put (P[i][j][0]);
            ofs.put (P[i][j][1]);
            ofs.put (P[i][j][2]);
          }
        ofs.close ();
      }

      {
        ofstream ofs ("pre.alpha.pgm", ios::out | ios::binary);
        ofs << "P5\n256 256\n255\n";
        for (int i = 0; i < 256; ++i)
          for (int j = 0; j < 256; ++j)
            ofs.put (P[i][j][3]);
        ofs.close ();
      }
#endif

      GLuint texture;
      glGenTextures (1, &texture);
      glBindTexture (GL_TEXTURE_2D, texture);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, P);

      return texture;
    }

    void Lut::load (const string & filename)
      throw (FileNotFound, ParseError)
    {
      ifstream ifs (filename.c_str ());
      if (! ifs) throw FileNotFound (filename.c_str ());

      for (int i = 0; i < 256; ++i)
      {
        GLuint r, g, b, a;
        if (! ifs >> r) throw ParseError::Type ("GLuint");
        if (! ifs >> g) throw ParseError::Type ("GLuint");
        if (! ifs >> b) throw ParseError::Type ("GLuint");
        if (! ifs >> a) throw ParseError::Type ("GLuint");
        m_data [i] = vec<AaUInt8> (r, g, b, a);
      }

      ifs.close ();
    }
  } // namespace R4d
} // namespace Aa

