#include "R3dMIP.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

    MIP::MIP (const std::string & vertex,
              const std::string & geometry,
              const std::string & fragment,
              GLenum mode) :
      ImageRenderer3dGLSL (vertex, geometry, fragment),
      m_mode (mode)
    {
      cout << "MIP\n";
    }

    MIP::~MIP ()
    {
    }

    void MIP::glPreDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

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

