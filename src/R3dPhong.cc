#include "R3dPhong.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    Phong::Phong (const std::string & fragment) :
      PostClassification (fragment),
      m_shininess (8.0),
      m_delta (vec (0.001f, 0.001f, 0.001f))
    {
    }

    Phong::~Phong ()
    {
    }

    void Phong::glPreDraw (const GL::CoreContext & context)
    {
      PostClassification::glPreDraw (context);
      m_program.set<GLfloat, 3, 3> ("aa_gl_normal_matrix",   GL::CoreContext::NormalMatrix (context.modelview ()));
      m_program.set<GLfloat>       ("aa_phong_shininess",    m_shininess);
      m_program.set<GLfloat>       ("aa_r3d_gradient_delta", m_delta);
    }

  } // namespace R3d
} // namespace Aa

