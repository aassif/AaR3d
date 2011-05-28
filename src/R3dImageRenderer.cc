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
    /*
    ** ImageRenderer.
    */

    ImageRenderer::ImageRenderer () : Aa::GL::Plugin ()
    {
    }

    ImageRenderer::~ImageRenderer()
    {
    }
  } // namespace R3d
} // namespace Aa

