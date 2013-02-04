#include "R3dBlinn.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

#if 0
    Blinn::Blinn (const std::string & vertex,
                  const std::string & geometry,
                  const std::string & fragment) :
      PostClassification (vertex, geometry, fragment),
      m_shininess (8.0),
      m_delta (vec (0.001f, 0.001f, 0.001f))
    {
    }
#endif

    Blinn::Blinn (const std::string & fragment) :
      PostClassification (fragment),
      m_shininess (8.0),
      m_delta (vec (0.001f, 0.001f, 0.001f))
    {
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

