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
      planes ()
    {
    }

    ImageRenderer::~ImageRenderer()
    {
    }

  } // namespace R3d
} // namespace Aa

