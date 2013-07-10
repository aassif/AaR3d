#include "R3dPhong.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    Phong::Phong (const std::string & fragment) :
      PostClassification (fragment),
      m_ambient   (0.0),
      m_diffuse   (1.0),
      m_specular  (0.5),
      m_shininess (16.0)
    {
    }

    Phong::~Phong ()
    {
    }

    void Phong::glPreDraw (const GL::CoreContext & c)
    {
      PostClassification::glPreDraw (c);
      GL::Location<mat3>  (m_program.location ("aa_gl_normal_matrix")) (GL::CoreContext::NormalMatrix (c.modelview ()));
      GL::Location<float> (m_program.location ("aa_phong_ambient"))    (m_ambient);
      GL::Location<float> (m_program.location ("aa_phong_diffuse"))    (m_diffuse);
      GL::Location<float> (m_program.location ("aa_phong_specular"))   (m_specular);
      GL::Location<float> (m_program.location ("aa_phong_shininess"))  (m_shininess);
    }

  } // namespace R3d
} // namespace Aa

