#include "R3dPostClassification.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

#if 0
    PostClassification::PostClassification (const std::string & vertex,
                                            const std::string & geometry,
                                            const std::string & fragment) :
      ImageRenderer3dGLSL (vertex, geometry, fragment),
      m_lut_tex1d ()
    {
      m_lut_tex1d [0] = 0;
      m_lut_tex1d [1] = 0;
    }
#endif

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

    void PostClassification::glPreDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      ImageRenderer3dGLSL::glPreDraw (motion);

      // Pre-multiplied blending.
      glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      // Texture #1 = lut1d.
      glActiveTexture (GL_TEXTURE1);
      glBindTexture (GL_TEXTURE_1D, m_lut_tex1d [motion ? 1 : 0]);
      m_program.set<GLint> ("lut1d", 1);

      //glActiveTexture (GL_TEXTURE0);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void PostClassification::glPostDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      // Blending.
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // Texture #1 = lut1d.
      glActiveTexture (GL_TEXTURE1);
      glBindTexture (GL_TEXTURE_1D, 0);

      ImageRenderer3dGLSL::glPostDraw (motion);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

