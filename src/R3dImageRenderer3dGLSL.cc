#include "R3dImageRenderer3dGLSL.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {
    ImageRenderer3dGLSL::ImageRenderer3dGLSL (const std::string & fragment) :
      ImageRenderer3d (),
      m_program ()
    {
      m_slicer = new SlicerViewCubeGLSLv2 (&m_program);
      m_program.attach (GL_FRAGMENT_SHADER, fragment);
      m_program.link ();
    }

    ImageRenderer3dGLSL::~ImageRenderer3dGLSL ()
    {
    }

    void ImageRenderer3dGLSL::glPreDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      ImageRenderer3d::glPreDraw (context);

      // Blending ON.
      //glEnable (GL_BLEND);

      m_program.use ();
      m_program.set<GLfloat, 4, 4> ("aa_gl_modelview",   context.modelview  ());
      m_program.set<GLfloat, 4, 4> ("aa_gl_projection",  context.projection ());
      //m_program.set<GLfloat, 2>    ("aa_gl_depth_range", context.depth_range ());
      m_program.set<GLint>         ("aa_r3d_tex3d", 0);
      m_program.set<GLfloat>       ("aa_r3d_min",   m_min);
      m_program.set<GLfloat>       ("aa_r3d_max",   m_max);
      //m_program.validate ();

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void ImageRenderer3dGLSL::glPostDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glUseProgram (0);
      //glDisable (GL_BLEND);
      ImageRenderer3d::glPostDraw (context);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

