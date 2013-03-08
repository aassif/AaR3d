#include <AaPrimitives>
#include "R3dMIP.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    vec4 MIP::FillColor (GLenum mode)
    {
      switch (mode)
      {
        case GL_MIN : return vec<float> (1, 1, 1);
        case GL_MAX : return vec<float> (0, 0, 0);
        default     : return vec<float> (1, 0, 0);
      }
    }

    MIP::MIP (GLenum mode) :
      ImageRenderer3dGLSL ("/Aa/R3d/Texture.fragment"),
      m_mode (mode),
      m_box (MIP::FillColor (mode))
    {
    }

    MIP::~MIP ()
    {
    }

    void MIP::glPreDraw (const GL::CoreContext & context)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glCullFace (GL_FRONT);
      m_box.glDraw (context);
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

