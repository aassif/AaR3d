#include "R3dImageRenderer.h"

using namespace std;
using namespace Aa::Math;

#ifdef DEBUG
#define ASSERT(x) assert (x)
#else
#define ASSERT(x)
#endif

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// ImageRenderer::ClippingPlane ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    ImageRenderer::ClippingPlane::ClippingPlane (const dvec3 & p, const dvec3 & n) :
      active (true),
      point (p),
      normal (n)
    {
    }

    bool ImageRenderer::ClippingPlane::test (const dvec3 & p) const
    {
      return DotProd (p - point, normal) >= 0;
    }

////////////////////////////////////////////////////////////////////////////////
// ImageRenderer ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    ImageRenderer::ImageRenderer () :
      Aa::GL::Plugin (),
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

