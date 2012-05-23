#include "R3dImageRenderer3dGLSL.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {
    ImageRenderer3dGLSL::ImageRenderer3dGLSL (const std::string & vertex,
                                              const std::string & geometry,
                                              const std::string & fragment) :
      ImageRenderer3d (),
      m_program ()
    {
      cout << "ImageRenderer3dGLSL (Powered by GLSL!)\n";

      //std::string vertex   = GL::Program::ReadSource ("PassThru.VertexShader.glsl");
      //std::string geometry = GL::Program::ReadSource ("McSlicing.GeometryShader.glsl");
      //std::string fragment = GL::Program::ReadSource ("Texture3d.FragmentShader.glsl");

      m_slicer = new SlicerViewCubeGLSLv2 (&m_program, vertex, geometry);
      m_program.attach (GL_FRAGMENT_SHADER, fragment);
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
      m_program.set<GLint> ("image3d", 0);

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

