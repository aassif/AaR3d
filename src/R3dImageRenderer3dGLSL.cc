#include "R3dImageRenderer3dGLSL.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {
#if 0
    ImageRenderer3dGLSL::ImageRenderer3dGLSL (const std::string & vertex,
                                              const std::string & geometry,
                                              const std::string & fragment) :
      ImageRenderer3d (),
      m_program (),
      m_min (0.0f),
      m_max (1.0f)
    {
      m_slicer = new SlicerViewCubeGLSLv2 (&m_program, vertex, geometry);
      m_program.attach (GL_FRAGMENT_SHADER, GL::Program::String (fragment));
      m_program.link ();
      //m_program.validate ();
    }
#endif

    ImageRenderer3dGLSL::ImageRenderer3dGLSL (const std::string & fragment) :
      ImageRenderer3d (),
      m_program ()
    {
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

      m_program.use ();
      m_program.set<GLint>         ("image_tex3d", 0);
      m_program.set<GLfloat, 4, 4> ("image_scale", Scale (m_image_scale));
      m_program.set<GLfloat>       ("image_min",   m_min);
      m_program.set<GLfloat>       ("image_max",   m_max);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void ImageRenderer3dGLSL::glPostDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glUseProgram (0);
      ImageRenderer3d::glPostDraw (motion);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

