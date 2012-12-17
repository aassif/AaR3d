#include <cmath>
#include <AaPrimitives>
#include "R3dImageRenderer3d.h"

using namespace std;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::ImageRenderer3d ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    ImageRenderer3d::ImageRenderer3d () :
      ImageRenderer (),
      m_slicer (NULL),
      m_tex3d (0),
      m_tex3d_shared (false),
      m_steps ()
    {
      m_steps [0] = 1.0f;
      m_steps [1] = 8.0f;
    }

    ImageRenderer3d::~ImageRenderer3d ()
    {
    }

    void ImageRenderer3d::setImage (const Image * img)
    {
      //cout << "--> ImageRenderer3d::setImg (this = " << this << ", img = " << img << ");" << endl;

      if (m_tex3d != 0)
      {
        if (! m_tex3d_shared)
          glDeleteTextures (1, &m_tex3d);

        m_tex3d = 0;
      }

      if (img != NULL)
      {
        GLuint tex3d = img->glTex3d ();
        this->setImage (tex3d, false);
      }

      //cout << "<-- ImageRenderer3d::setImg (this = " << this << ", img = " << img << ");" << endl;
    }

    void ImageRenderer3d::setImage (GLuint tex3d, bool shared)
    {
      m_tex3d        = tex3d;
      m_tex3d_shared = shared;
    }

    void ImageRenderer3d::setLut (const Lut *)
    {
      //cout << "--> ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;
      //cout << "<-- ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;
    }

    double ImageRenderer3d::step (bool motion) const
    {
      return m_steps [motion ? 1 : 0];
    }

    void ImageRenderer3d::setStep (bool motion, double step)
    {
      m_steps [motion ? 1 : 0] = step;
    }

    void ImageRenderer3d::glDraw (bool motion)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      this->glPreDraw (motion);

      if (m_slicer != NULL)
        m_slicer->glDraw (m_steps [motion ? 1 : 0]);

      this->glPostDraw (motion);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void ImageRenderer3d::glPreDraw (bool)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      // Bind 1st texture (image).
      glActiveTexture (GL_TEXTURE0);
      glBindTexture (GL_TEXTURE_3D, m_tex3d);

      // Lighting OFF.
      glDisable (GL_LIGHTING);
      // Blending ON.
      glEnable (GL_BLEND);
      //glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      //glDepthMask (GL_FALSE); //FIXME

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

    void ImageRenderer3d::glPostDraw (bool)
    {
      //cout << "--> " << __PRETTY_FUNCTION__ << endl;

      glDisable (GL_BLEND);
      //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      //glDepthMask (GL_TRUE);

      glActiveTexture (GL_TEXTURE0);
      glBindTexture (GL_TEXTURE_3D, 0);

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

