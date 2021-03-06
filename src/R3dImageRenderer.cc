#include "R3dImageRenderer.h"

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// ImageRenderer::ClippingPlane ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    ImageRenderer::ClippingPlane::ClippingPlane (const vec3 & p, const vec3 & n) :
      active (true),
      point (p),
      normal (n)
    {
    }

    bool ImageRenderer::ClippingPlane::test (const vec3 & p) const
    {
      return DotProd (p - point, normal) >= 0;
    }

////////////////////////////////////////////////////////////////////////////////
// ImageRenderer ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    ImageRenderer::ImageRenderer () :
      planes (),
      m_min (0.0f),
      m_max (1.0f)
    {
    }

    ImageRenderer::~ImageRenderer()
    {
    }

    void ImageRenderer::setRange (GLfloat min, GLfloat max)
    {
      m_min = min;
      m_max = max;
    }

  } // namespace R3d
} // namespace Aa

