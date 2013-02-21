#include "R3dRainbow.h"

namespace Aa
{
  namespace R3d
  {

    Rainbow::Rainbow () :
      PostClassification ("/Aa/R3d/Rainbow.fragment"),
      m_delta (vec (0.001f, 0.001f, 0.001f))
    {
    }

    Rainbow::~Rainbow ()
    {
    }

    void Rainbow::glPreDraw (const GL::CoreContext & context)
    {
      PostClassification::glPreDraw (context);
      m_program.set<GLfloat> ("aa_r3d_rainbow_delta", m_delta);
    }

  } // namespace R3d
} // namespace Aa

