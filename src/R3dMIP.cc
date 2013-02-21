#include <AaPrimitives>
#include "R3dMIP.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    MIP::MIP (GLenum mode) :
      ImageRenderer3dGLSL ("/Aa/R3d/Texture.fragment"),
      m_mode (mode)
    {
    }

    MIP::~MIP ()
    {
    }

    void MIP::glPreDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glCullFace (GL_FRONT);
      glColor3f (0.0f, 0.0f, 0.0f);
      glLoadIdentity ();
      GL::MultMatrix (context.modelview ());
      GL::Primitives::Box ();
      glCullFace (GL_BACK);

      ImageRenderer3dGLSL::glPreDraw (context);
      glBlendEquation (m_mode);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void MIP::glPostDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glBlendEquation (GL_FUNC_ADD);
      ImageRenderer3dGLSL::glPostDraw (context);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

