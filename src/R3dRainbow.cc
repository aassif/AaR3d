#include "R3dRainbow.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {
#if 0
    Rainbow::Rainbow (const std::string & vertex,
                      const std::string & geometry,
                      const std::string & fragment) :
      PostClassification (vertex, geometry, fragment),
      m_delta (vec (0.001f, 0.001f, 0.001f))
    {
      cout << "Rainbow (Powered by GLSL!)\n";
    }
#endif

    Rainbow::Rainbow () :
      PostClassification ("/AaR3d/Rainbow.fragment"),
      m_delta (vec (0.001f, 0.001f, 0.001f))
    {
      cout << "Rainbow (Powered by GLSL!)\n";
    }

    Rainbow::~Rainbow ()
    {
    }

    void Rainbow::glPreDraw (bool motion)
    {
      PostClassification::glPreDraw (motion);
      m_program.set<GLfloat> ("delta", m_delta);
    }

  } // namespace R3d
} // namespace Aa

