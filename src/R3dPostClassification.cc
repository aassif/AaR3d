#include "R3dPostClassification.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    PostClassification::PostClassification (const std::string & fragment) :
      ImageRenderer3dGLSL (fragment),
      m_lut_tex1d ()
    {
      m_lut_tex1d [0] = 0;
      m_lut_tex1d [1] = 0;
    }

    PostClassification::~PostClassification ()
    {
    }

    void PostClassification::setLut (const Lut * lut)
    {
      //cout << "--> ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;

      glDeleteTextures (2, m_lut_tex1d);

      static const Lut DEFAULT = Lut ();
      const Lut * l = (lut != NULL ? lut : &DEFAULT);

      m_lut_tex1d [0] = l->glTex1d (m_steps [0]);
      m_lut_tex1d [1] = l->glTex1d (m_steps [1]);

      //cout << "<-- ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;
    }

    void PostClassification::glPreDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      ImageRenderer3dGLSL::glPreDraw (context);

      // Pre-multiplied blending.
      glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      glActiveTexture (GL_TEXTURE1);
      glBindTexture (GL_TEXTURE_1D, m_lut_tex1d [context.is_moving () ? 1 : 0]);
      m_program.set<GLint> ("aa_r3d_lut1d", 1);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void PostClassification::glPostDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      // Blending.
      glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                           GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);

      glActiveTexture (GL_TEXTURE1);
      glBindTexture (GL_TEXTURE_1D, 0);

      ImageRenderer3dGLSL::glPostDraw (context);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

