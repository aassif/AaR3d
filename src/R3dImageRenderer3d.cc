#include <cmath>
#include <AaPrimitives>
#include "R3dPadding.h"
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
      m_box (),
      m_image_tex3d (0),
      m_image_scale (1.0),
      m_steps ()
    {
      m_steps [0] = 1.0f;
      m_steps [1] = 8.0f;
    }

    ImageRenderer3d::~ImageRenderer3d ()
    {
    }

    void ImageRenderer3d::setImg (const Image * img)
    {
      //cout << "--> ImageRenderer3d::setImg (this = " << this << ", img = " << img << ");" << endl;

      if (m_image_tex3d != 0)
      {
        glDeleteTextures (1, &m_image_tex3d);
        m_image_tex3d = 0;
        m_image_scale = dvec3 (1.0);
      }

      if (img != NULL)
      {
        const unsigned short d1x = img->dx ();
        const unsigned short d1y = img->dy ();
        const unsigned short d1z = img->dz ();

        unsigned short d2x = (1 << (unsigned short) ceil (log2 (d1x)));
        unsigned short d2y = (1 << (unsigned short) ceil (log2 (d1y)));
        unsigned short d2z = (1 << (unsigned short) ceil (log2 (d1z)));

        dvec3 scale = vec<double> ((double) d1x / d2x, (double) d1y / d2y, (double) d1z / d2z);

        //cout << d1x << 'x' << d1y << 'x' << d1z << " --> "
        //     << d2x << 'x' << d2y << 'x' << d2z << endl;

        const unsigned char * b1 = (const unsigned char *) img->begin ();
        const unsigned char * b2 = pad3d (d1x, d1y, d1z, b1, d2x, d2y, d2z);

        GLuint tex3d = 0;
        glGenTextures (1, &tex3d);
        glBindTexture (GL_TEXTURE_3D, tex3d);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
        glTexImage3D (GL_TEXTURE_3D, 0,
                      GL_R8, d2x, d2y, d2z, 0,
                      GL_RED, GL_UNSIGNED_BYTE, b2);
        //cout << "glTexImage3D : " << gluErrorString (glGetError ()) << endl;
        glGenerateMipmap (GL_TEXTURE_3D);

        delete[] b2;

        this->setImg (img->box (), tex3d, scale);
      }

      //cout << "<-- ImageRenderer3d::setImg (this = " << this << ", img = " << img << ");" << endl;
    }

    void ImageRenderer3d::setImg (const dbox3 & box, GLuint tex3d, const dvec3 & scale)
    {
      m_box         = box;
      m_image_tex3d = tex3d;
      m_image_scale = scale;
    }

    void ImageRenderer3d::setLut (const Lut *)
    {
      //cout << "--> ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;
      //cout << "<-- ImageRenderer3d::setLut (this = " << this << ", lut = " << lut << ");" << endl;
    }

    double ImageRenderer3d::getStep (bool motion) const
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

      glPushMatrix ();
      GL::Translate (m_box.pos ());
      GL::Scale     (m_box.dim ());

      // Bind 1st texture (image).
      glActiveTexture (GL_TEXTURE0);
      glBindTexture (GL_TEXTURE_3D, m_image_tex3d);
      glMatrixMode (GL_TEXTURE);
      glPushMatrix ();
      //glLoadIdentity ();
      GL::Scale (m_image_scale);

      glMatrixMode (GL_MODELVIEW);

      // Activate texture unit #0.
      glActiveTexture (GL_TEXTURE0);

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
      glMatrixMode (GL_TEXTURE);
      glPopMatrix ();

      glMatrixMode (GL_MODELVIEW);
      glPopMatrix ();

      //cout << "<-- " << __PRETTY_FUNCTION__ << endl;
    }

  } // namespace R3d
} // namespace Aa

