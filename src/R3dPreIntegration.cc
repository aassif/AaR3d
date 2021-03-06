//#include <cmath>
//#include <AaGLSL>
#include "R3dPreIntegration.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    GLuint isosurface (int k)
    {
      GLubyte P [256][256][4];

      for (int i = 0; i < 256; ++i)
        for (int j = 0; j < 256; ++j)
        {
          P [i][j][0] = 255;
          P [i][j][1] = 0;
          P [i][j][2] = 0;
          P [i][j][3] = ((i <= k && k <= j) || (j <= k && k <= i)) ? 255 : 0;
        }

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

#if 0
    PreIntegration::PreIntegration (const std::string & vertex,
                                    const std::string & geometry,
                                    const std::string & fragment) :
      ImageRenderer3dGLSL (vertex, geometry, fragment),
      m_lut_tex2d ()
    {
      cout << "PreIntegration (Powered by GLSL!)\n";
      m_lut_tex2d [0] = 0;
      m_lut_tex2d [1] = 0;
    }
#endif

    PreIntegration::PreIntegration (const std::string & fragment) :
      ImageRenderer3dGLSL (fragment),
      m_lut_tex2d ()
    {
      m_lut_tex2d [0] = 0;
      m_lut_tex2d [1] = 0;
    }

    PreIntegration::~PreIntegration ()
    {
    }

    void PreIntegration::setLut (const Lut * lut)
    {
      //cout << "--> PreIntegration::setLut (this = " << this << ", lut = " << lut << ");" << endl;

      glDeleteTextures (2, m_lut_tex2d);

      static const Lut DEFAULT = Lut ();
      const Lut * l = (lut != NULL ? lut : &DEFAULT);

#if 1
      m_lut_tex2d [0] = l->glTex2d (m_steps [0]);
      m_lut_tex2d [1] = l->glTex2d (m_steps [1]);
#else
      m_lut_tex2d [0] = isosurface (64);
      m_lut_tex2d [1] = isosurface (64);
#endif

      //cout << "<-- PreIntegration::setLut (this = " << this << ", lut = " << lut << ");" << endl;
    }

    void PreIntegration::glPreDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      ImageRenderer3dGLSL::glPreDraw (context);

      // Blending ON.
      glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      GLSL::Location<mat4>  (m_program.location ("aa_gl_modelview_inverse")) (context.modelview ().inv ());

      glActiveTexture (GL_TEXTURE1);
      glBindTexture (GL_TEXTURE_2D, m_lut_tex2d [context.is_moving () ? 1 : 0]);
      GLSL::Location<int>   (m_program.location ("aa_r3d_lut2d")) (1);
      GLSL::Location<float> (m_program.location ("aa_r3d_step"))  (m_steps [context.is_moving () ? 1 : 0]);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void PreIntegration::glPostDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                           GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);

      ImageRenderer3dGLSL::glPostDraw (context);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

