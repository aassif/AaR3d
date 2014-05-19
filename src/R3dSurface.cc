//#include <cmath>
//#include <AaGLSL>
#include "R3dSurface.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    GLuint isosurface (AaUInt k, const vec4 & color)
    {
      vec4 T [256][256];

      for (AaUInt i = 0; i < 256; ++i)
        for (AaUInt j = 0; j < 256; ++j)
        {
          bool h = (i <= k && k <= j) || (j <= k && k <= i);
          T[i][j] = h ? color : vec4 ();
        }

      GLuint texture;
      glGenTextures (1, &texture);
      glBindTexture (GL_TEXTURE_2D, texture);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_FLOAT, T);

      return texture;
    }

    Surface::Surface (const std::string & fragment) :
      ImageRenderer3dGLSL (fragment),
      m_iso (8),
      m_color (vec<float> (1, 0, 0, 1)),
      m_lut_tex2d (isosurface (m_iso, m_color))
    {
    }

    Surface::~Surface ()
    {
    }

    void Surface::setLut (const Lut * lut)
    {
      //cout << "--> Surface::setLut (this = " << this << ", lut = " << lut << ");" << endl;

      glDeleteTextures (1, &m_lut_tex2d);
      m_lut_tex2d = isosurface (m_iso, m_color);

      //cout << "<-- Surface::setLut (this = " << this << ", lut = " << lut << ");" << endl;
    }

    void Surface::glPreDraw (const GL::CoreContext & c)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      ImageRenderer3dGLSL::glPreDraw (c);

      GLSL::Location<mat4> (m_program.location ("aa_gl_modelview_inverse")) (c.modelview ().inv ());
      GLSL::Location<mat3> (m_program.location ("aa_gl_normal_matrix"))     (GL::CoreContext::NormalMatrix (c.modelview ()));

      glActiveTexture (GL_TEXTURE1);
      glBindTexture (GL_TEXTURE_2D, m_lut_tex2d);
      GLSL::Location<GLint>   (m_program.location ("aa_r3d_lut2d"))       (1);
      GLSL::Location<GLfloat> (m_program.location ("aa_r3d_step"))        (m_steps [c.is_moving () ? 1 : 0]);
      GLSL::Location<GLfloat> (m_program.location ("aa_phong_shininess")) (16);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

