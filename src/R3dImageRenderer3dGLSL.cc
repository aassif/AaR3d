#include "R3dImageRenderer3dGLSL.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {
    ImageRenderer3dGLSL::ImageRenderer3dGLSL (const std::string & fragment) :
      ImageRenderer3d (),
      m_program ()
    {
      if (! GLEW_ARB_shading_language_include)
        throw GL::MissingExtension ("GL_ARB_shading_language_include");

      m_slicer = new SlicerViewCubeGLSLv2 (&m_program);
      m_program.attach (GL_FRAGMENT_SHADER, GL::Program::String (fragment));
      m_program.link ();
      //m_program.validate ();
    }

    ImageRenderer3dGLSL::~ImageRenderer3dGLSL ()
    {
    }

    void ImageRenderer3dGLSL::glPreDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      ImageRenderer3d::glPreDraw (motion);

      // Blending ON.
      glEnable (GL_BLEND);

      m_program.use ();
      m_program.set<GLint>   ("image_tex3d", 0);
      m_program.set<GLfloat> ("image_min",   m_min);
      m_program.set<GLfloat> ("image_max",   m_max);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void ImageRenderer3dGLSL::glPostDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glUseProgram (0);
      //glDisable (GL_BLEND);
      ImageRenderer3d::glPostDraw (motion);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

