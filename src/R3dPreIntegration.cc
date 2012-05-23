//#include <cmath>
//#include <AaGLSL>
#include "R3dPreIntegration.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {
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

    PreIntegration::~PreIntegration ()
    {
    }

    void PreIntegration::setLut (const Lut * lut)
    {
      //cout << "--> PreIntegration::setLut (this = " << this << ", lut = " << lut << ");" << endl;

      glDeleteTextures (2, m_lut_tex2d);

      static const Lut DEFAULT = Lut ();
      const Lut * l = (lut != NULL ? lut : &DEFAULT);

      m_lut_tex2d [0] = l->glTex2d (m_steps [0]);
      m_lut_tex2d [1] = l->glTex2d (m_steps [1]);

      //cout << "<-- PreIntegration::setLut (this = " << this << ", lut = " << lut << ");" << endl;
    }

    void PreIntegration::glPreDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      ImageRenderer3dGLSL::glPreDraw (motion);

      // Blending ON.
      glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      // Texture #1 = lut2d.
      glActiveTexture (GL_TEXTURE1);
      glBindTexture (GL_TEXTURE_2D, m_lut_tex2d [motion ? 1 : 0]);
      //glActiveTexture (GL_TEXTURE0);
      m_program.set<GLint>   ("lut2d",           1);
      m_program.set<GLfloat> ("mc_slicing_step", m_steps [motion ? 1 : 0]);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void PreIntegration::glPostDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      ImageRenderer3dGLSL::glPostDraw (motion);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

