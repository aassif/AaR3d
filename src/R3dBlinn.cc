#include "R3dBlinn.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

    Blinn::Blinn (const std::string & vertex,
                  const std::string & geometry,
                  const std::string & fragment) :
      PostClassification (vertex, geometry, fragment),
      m_shininess (8.0),
      m_delta (vec (0.001f, 0.001f, 0.001f))
    {
      cout << "Blinn (Powered by GLSL!)\n";
    }

    Blinn::~Blinn ()
    {
    }

    void Blinn::glPreDraw (bool motion)
    {
      PostClassification::glPreDraw (motion);
      m_program.set<GLfloat> ("shininess", m_shininess);
      m_program.set<GLfloat> ("delta",     m_delta);
    }

  } // namespace R3d
} // namespace Aa

