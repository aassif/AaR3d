#include <AaPrimitives>
#include "R3dMIP.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

#if 0
    MIP::MIP (const std::string & vertex,
              const std::string & geometry,
              const std::string & fragment,
              GLenum mode) :
      ImageRenderer3dGLSL (vertex, geometry, fragment),
      m_mode (mode)
    {
    }
#endif

    MIP::MIP (GLenum mode) :
      ImageRenderer3dGLSL ("/AaR3d/Texture3d.fragment"),
      m_mode (mode)
    {
    }

    MIP::~MIP ()
    {
    }

    void MIP::glPreDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glCullFace (GL_FRONT);
      glColor3f (0.0f, 0.0f, 0.0f);
      GL::Primitives::Box ();
      glCullFace (GL_BACK);

      ImageRenderer3dGLSL::glPreDraw (motion);
      glBlendEquation (m_mode);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void MIP::glPostDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glBlendEquation (GL_FUNC_ADD);
      ImageRenderer3dGLSL::glPostDraw (motion);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

